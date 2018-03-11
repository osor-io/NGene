#include "EntityManager.h"
#include "../_component/ComponentManager.h"
#include "../_system/SystemManager.h"
#include "../lua/LuaManager.h"

EntityManager::EntityManager() {}


EntityManager::~EntityManager() {}

void EntityManager::startUp() {
    Entity::exposeToLua();
    exposeToLua();
}

void EntityManager::shutDown() {
    m_entities.clear();
}

Entity * EntityManager::loadEntity(const sol::table & table, const std::string & name)
{
    auto e = new Entity(m_nextId++);
    e->setType(name);
    const sol::table componentTable = table[name];
    componentTable.for_each([&e](const auto& key, const auto& value) {

        if (key.is<std::string>()) {
            auto component_name = key.as<std::string>();
            ComponentManager::get().addComponentToEntity(*e, component_name, value.as<sol::table>());
        }
        else {
            // The key was not a string, we will ignore it.
        }

    });

    m_entities[e->getId()] = std::unique_ptr<Entity>(e);

    SystemManager::get().registerEntityInSystems(*e);

    return e;
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
