#include "EntityManager.h"
#include "../_component/ComponentManager.h"
#include "../_system/SystemManager.h"
#include "../lua/LuaManager.h"
#include <sstream>
#include "../utils/File.h"
#include "../debug/LoggingManager.h"
#include "../resources/TextFileManager.h"
#include <cassert>
#include "File.h"
#include "json.hpp"
#include "../physics/ChunkManager.h"

EntityManager::EntityManager() {}


EntityManager::~EntityManager() {}

void EntityManager::start_up() {
    //set_scripts();
}

void EntityManager::shut_down() {
    m_entities.clear();
}

void EntityManager::set_scripts() {

    Entity::expose_to_lua();
    expose_to_lua();

    assert(TextFileManager::get().exists_resource("res/scripts/CustomTypes.lua"));
    assert(TextFileManager::get().exists_resource("res/scripts/EngineTypes.lua"));


    auto custom_types_script = TextFileManager::get().get_scoped_resource("res/scripts/CustomTypes.lua");
    auto engine_types_script = TextFileManager::get().get_scoped_resource("res/scripts/EngineTypes.lua");

    LUA.script(*custom_types_script.resource);
    LUA.script(*engine_types_script.resource);

}

void EntityManager::update_entities() {

    if (m_requested_clear) {
        m_requested_clear = false;

        /*
        First we clear all the entities currently in the system.
        */
        for (auto& e : m_entities) {
            assert(e.second->get_id() == e.first);
            SystemManager::get().deregister_entity_in_systems(e.second->get_id());
        }
        m_entities.clear();
        ChunkManager::get().clear();
        m_next_id = 0;

        if (m_requested_load) {

            if (m_requested_script_reload) {
                LuaManager::get().reset_state();
                m_requested_script_reload = false;
            }

            m_requested_load = false;
            /*
            We create the entities depending on their type and
            then load their data from the json file.
            */
            for (const auto& json_entity : m_unprocessed_load_entity_data["Entities"]) {
                auto e = create_entity_internal(json_entity["type"], LUA["Entities"]);
                e->load_json(json_entity);
                m_entities[e->get_id()] = std::unique_ptr<Entity>(e);
                SystemManager::get().register_entity_in_systems(*e);
            }
        }

        return;
    }
    else {

        for (auto entity : m_entities_to_add) {
            m_entities[entity->get_id()] = std::unique_ptr<Entity>(entity);
            SystemManager::get().register_entity_in_systems(*entity);
        }
        m_entities_to_add.clear();

        for (auto id : m_entity_ids_to_remove) {
            SystemManager::get().deregister_entity_in_systems(id);
            m_entities.erase(id);
        }
        m_entity_ids_to_remove.clear();
    }

    /*
    Additional entities can be requested wether we have loaded
    entities from json or directly with the exposed funcions.
    */
    for (auto entity : m_additional_entities_to_add) {
        m_entities[entity->get_id()] = std::unique_ptr<Entity>(entity);
        SystemManager::get().register_entity_in_systems(*entity);
    }
    m_additional_entities_to_add.clear();

}

EntityId EntityManager::request_load_entity(const std::string & type, const sol::table & table, bool request_script_reload)
{
    if (!m_requested_script_reload)
        m_requested_script_reload = request_script_reload;

    auto e = create_entity_internal(type, table);

    m_entities_to_add.push_back(e);
    return e->get_id();
}

Entity * EntityManager::create_additional_engine_entity(const std::string & type) {
    auto e = create_entity_internal(type, LUA["EngineEntities"]);
    e->m_serializable = false;
    m_additional_entities_to_add.push_back(e);
    return e;
}

void EntityManager::remove_entity(EntityId id) {
    m_entity_ids_to_remove.push_back(id);
}

bool EntityManager::has_entity(EntityId id) const {
    auto it = m_entities.find(id);
    if (it != m_entities.end()) {
        return true;
    }
    return false;
}

Entity* EntityManager::get_entity(EntityId id) {
    auto it = m_entities.find(id);
    if (it != m_entities.end()) {
        return (it->second.get());
    }
    return nullptr;
}

size_t EntityManager::entity_size() const {
    return m_entities.size();
}

std::vector<EntityId> EntityManager::get_entity_keys() const {
    auto vec = std::vector<EntityId>();
    vec.resize(m_entities.size());
    auto i = 0;
    for (const auto& e : m_entities) {
        vec[i++] = e.first;
    }
    return vec;
}

EntityManager::OwningEntitiesGroup & EntityManager::get_entities() {
    return m_entities;
}


json EntityManager::serialize_entities() const {
    auto j = json{};

    auto entities = std::vector<json>{};

    for (const auto& e : m_entities) {
        if (e.second->m_serializable)
            entities.push_back(e.second->to_json());
    }

    j["Entities"] = entities;

    return j;
}

void EntityManager::serialize_entities_to_file(const char* filename) const {
    auto j = serialize_entities();
    write_to_file(filename, j.dump().c_str());
}

void EntityManager::clear_and_load_entities_from_file(const char * filename) {
    auto s = read_from_file(filename);
    auto j = json::parse(s);
    EntityManager::get().clear_and_load_entities(j);
}


void EntityManager::clear_entities() {
    if (m_requested_clear) return;
    m_requested_clear = true;

}

void EntityManager::clear_and_load_entities(const json& j) {

    if (m_requested_clear || m_requested_load) {
        return;
    }
    else {

        m_requested_clear = true;
        m_requested_load = true;
        m_requested_script_reload = true;

        m_unprocessed_load_entity_data = j;
    }
}

void EntityManager::reregister_entity(EntityId id) {
    auto entity = get_entity(id);
    SystemManager::get().deregister_entity_in_systems(id);
    SystemManager::get().register_entity_in_systems(*entity);
}

void EntityManager::expose_to_lua() {

    LUA.set_function("hasEntity", [this](EntityId id) -> bool {
        return has_entity(id);
    });


    LUA.set_function("getEntity", [this](EntityId id) -> Entity* {
        return get_entity(id);
    });


    LUA.set_function("getEntitySafe", [this](EntityId id) -> Entity* {
        if (has_entity(id)) {
            return get_entity(id);
        }
        LOGF("Lua is trying to access an entity that doesn't exist");
        std::terminate();
    });

}

Entity * EntityManager::create_entity_internal(const std::string & type, const sol::table & table) {

    auto id = m_next_id++;

    auto ss = std::stringstream{};
    ss << type << "_" << id;

    auto e = new Entity(id, ss.str(), type);

    const sol::table componentTable = table[type];
    componentTable.for_each([&e](const auto& key, const auto& value) {

        if (key.is<std::string>()) {
            auto component_name = key.as<std::string>();
            assert(ComponentManager::get().supports_component(component_name));
            ComponentManager::get().add_component_to_entity(*e, component_name, value.as<sol::table>());
        }
        else {
            // The key was not a string, we will ignore it.
        }

    });

    return e;
}

