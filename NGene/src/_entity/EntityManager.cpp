#include "EntityManager.h"
#include "../_component/ComponentManager.h"
#include "../_system/SystemManager.h"
#include "../lua/LuaManager.h"
#include <sstream>

EntityManager::EntityManager() {}


EntityManager::~EntityManager() {}

void EntityManager::startUp() {
    Entity::exposeToLua();
    exposeToLua();
}

void EntityManager::shutDown() {
    m_entities.clear();
}

void EntityManager::updateEntities() {

    for (auto entity : m_entitiesToAdd) {
        m_entities[entity->getId()] = std::unique_ptr<Entity>(entity);
        SystemManager::get().registerEntityInSystems(*entity);
    }
    m_entitiesToAdd.clear();

    for (auto id : m_entitieIdsToRemove) {
        SystemManager::get().deregisterEntityInSystems(id);
        m_entities.erase(id);
    }
    m_entitieIdsToRemove.clear();

}


EntityId EntityManager::loadEntity(const sol::table & table, const std::string & name)
{
    auto id = m_nextId++;

    auto ss = std::stringstream{};
    ss << name << "_" << id;

    auto e = new Entity(id, ss.str(), name);

    const sol::table componentTable = table[name];
    componentTable.for_each([&e](const auto& key, const auto& value) {

        if (key.is<std::string>()) {
            auto component_name = key.as<std::string>();
            // @@TODO: Check if component name exists
            ComponentManager::get().addComponentToEntity(*e, component_name, value.as<sol::table>());
        }
        else {
            // The key was not a string, we will ignore it.
        }

    });

    m_entitiesToAdd.push_back(e);
    return id;
}

void EntityManager::removeEntity(EntityId id) {
    m_entitieIdsToRemove.push_back(id);
}

bool EntityManager::hasEntity(EntityId id) const {
    auto it = m_entities.find(id);
    if (it != m_entities.end()) {
        return true;
    }
    return false;
}

Entity* EntityManager::getEntity(EntityId id) {
    auto it = m_entities.find(id);
    if (it != m_entities.end()) {
        return (it->second.get());
    }
    return nullptr;
}

size_t EntityManager::numberOfEntities() const {
    return m_entities.size();
}

std::vector<EntityId> EntityManager::getEntityKeys() const {
    auto vec = std::vector<EntityId>();
    vec.resize(m_entities.size());
    auto i = 0;
    for (const auto& e : m_entities) {
        vec[i++] = e.first;
    }
    return vec;
}

void EntityManager::exposeToLua() {

    LUA.set_function("hasEntity", [this](EntityId id) -> bool {
        return hasEntity(id);
    });


    LUA.set_function("getEntity", [this](EntityId id) -> Entity* {
        return getEntity(id);
    });


    LUA.set_function("getEntitySafe", [this](EntityId id) -> Entity* {
        if (hasEntity(id)) {
            return getEntity(id);
        }
        LOG("Lus is trying to access an entity that doesn't exist");
        std::terminate();
    });

}

