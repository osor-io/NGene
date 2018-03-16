#include "EntityManager.h"
#include "../_component/ComponentManager.h"
#include "../_system/SystemManager.h"
#include "../lua/LuaManager.h"
#include <sstream>
#include "../utils/File.h"

EntityManager::EntityManager() {}


EntityManager::~EntityManager() {}

void EntityManager::start_up() {

    /*
    Load here all the entity types
    */
    LUA.script(R"(
            Entities = {
                Cosa = {
                    TransformComponent = {
                        x = 0,
                        y = 10
                    },
                    PhraseComponent = {
                        phrase = "I'm saying hi from this lua Object!! :D"
                    },
                    SpriteComponent = {
                        filename = "C:/tmp/test.png"
                    },
                    BehaviourComponent = {
                        onUpdate = function (this, deltaTime)
                            transform = this:getTransformComponent()
                            local x = getAxisPosition(Axis.X)
                            local y = getAxisPosition(Axis.Y)
                            if(transform) then
                                transform.position.x = transform.position.x + (x * 3 * deltaTime) 
                                transform.position.y = transform.position.y + (y * 3 * deltaTime) 
                            else    
                                print("We got no transform")
                            end
                            --[[
                            print("X:", getAxisPosition(Axis.X))
                            print("Y:", getAxisPosition(Axis.Y))
                            print("Z:", getAxisPosition(Axis.Z))
                            print("R:", getAxisPosition(Axis.R))
                            print("U:", getAxisPosition(Axis.U))
                            print("V:", getAxisPosition(Axis.V))
                            print("PovX:", getAxisPosition(Axis.PovX))
                            print("PovY:", getAxisPosition(Axis.PovY))
                            --]]
                        end
                    },
                    InputComponent = {
                        onKeyUp = function (this, button)
                            print("Down: ", button)
                            if(button == Key.A) then
                                print("Key was an A")
                            else
                                print("Key was NOT an A")
                            end
                        end,
                        onKeyDown = function (this, button)
                            print("Up: ", button)
                        end,
                        onButtonUp = function (this, button)
                            print("Button Up: ", button)
                        end,
                        onButtonDown = function (this, button)
                            transform = this:getTransformComponent()  
                            if(transform) then
                                print("Accessing our transform: ", transform.position.y) 
                            end
                            print("Button Down: ", button)
                        end,
                        forLeftJoystick = function (this, x, y)
                            local deltaTime = getDeltaTime()
                            transform = this:getTransformComponent()
                            if(transform) then
                                transform.position.x = transform.position.x + (x * 3 * deltaTime) 
                                transform.position.y = transform.position.y + (y * 3 * deltaTime) 
                            end
                        end,
                        forRightJoystick = function (this, x, y)
                            local deltaTime = getDeltaTime()
                            transform = this:getTransformComponent()
                            if(transform) then
                                transform.position.x = transform.position.x + (x * 3 * deltaTime) 
                                transform.position.y = transform.position.y + (y * 3 * deltaTime) 
                            end
                        end
                    }
                },
                OtraCosa = {
                    PhraseComponent = {
                        phrase = "I'm saying hi from this lua Object!! :D"
                    },
                    SpriteComponent = {
                        filename = "file.png"
                    }
                },
                YOtraMas = {
                    PhraseComponent = {
                        phrase = "I'm saying hi from this lua Object!! :D"
                    },
                    SpriteComponent = {
                        filename = "file.png"
                    }
                }
            }
        )");


    Entity::expose_to_lua();
    expose_to_lua();
}

void EntityManager::shut_down() {
    m_entities.clear();
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
        m_next_id = 0;

        if (m_requested_load) {
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
}


EntityId EntityManager::load_entity(const std::string & type, const sol::table & table)
{
    auto e = create_entity_internal(type, table);

    m_entities_to_add.push_back(e);
    return e->get_id();
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


json EntityManager::serialize_entities() const {
    auto j = json{};

    auto entities = std::vector<json>{};

    for (const auto& e : m_entities) {
        entities.push_back(e.second->to_json());
    }

    j["Entities"] = entities;

    return j;
}

void EntityManager::serialize_entities_to_file(const char* filename) const {
    auto j = serialize_entities();
    write_to_file(filename, j.dump().c_str());
}


void EntityManager::clear_entities() {
    if (m_requested_clear) return;
    m_requested_clear = true;

}

void EntityManager::clear_and_load_entities(const json& j) {
    if (m_requested_clear || m_requested_load) return;
    m_requested_clear = true;
    m_requested_load = true;

    m_unprocessed_load_entity_data = j;
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
        LOG("Lus is trying to access an entity that doesn't exist");
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
            // @@TODO: Check if component name exists
            ComponentManager::get().add_component_to_entity(*e, component_name, value.as<sol::table>());
        }
        else {
            // The key was not a string, we will ignore it.
        }

    });

    return e;
}

