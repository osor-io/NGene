#include "EntityManager.h"
#include "../_component/ComponentManager.h"
#include "../_system/SystemManager.h"
#include "../lua/LuaManager.h"
#include <sstream>

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
                            if(transform) then
                                transform.position.y = transform.position.y + (10 * deltaTime) 
                            else    
                                print("We got no transform")
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


    Entity::exposeToLua();
    exposeToLua();
}

void EntityManager::shut_down() {
    m_entities.clear();
}

void EntityManager::updateEntities() {

    if (m_requestedClearAndLoad) {
        m_requestedClearAndLoad = false;
        
        /*
        First we clear all the entities currently in the system.
        */
        for (auto& e : m_entities) {
            assert(e.second->getId() == e.first);
            SystemManager::get().deregisterEntityInSystems(e.second->getId());
        }
        m_entities.clear();
        m_nextId = 0;

        /*
        We create the entities depending on their type and
        then load their data from the json file.
        */
        for (const auto& json_entity : m_loadEntityData["Entities"]) {
            auto e = createEntityInternal(json_entity["type"], LUA["Entities"]);
            e->loadJson(json_entity);
            m_entities[e->getId()] = std::unique_ptr<Entity>(e);
            SystemManager::get().registerEntityInSystems(*e);
        }
        

        return;
    }
    else {

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
}


EntityId EntityManager::loadEntity(const std::string & type, const sol::table & table)
{
    auto e = createEntityInternal(type, table);

    m_entitiesToAdd.push_back(e);
    return e->getId();
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


json EntityManager::serializeEntities() const {
    auto j = json{};

    auto entities = std::vector<json>{};

    for (const auto& e : m_entities) {
        entities.push_back(e.second->toJson());
    }

    j["Entities"] = entities;

    return j;
}

void EntityManager::clearAndloadEntities(const json& j) {
    if (m_requestedClearAndLoad) return;

    m_requestedClearAndLoad = true;

    m_loadEntityData = j;

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

Entity * EntityManager::createEntityInternal(const std::string & type, const sol::table & table){

    auto id = m_nextId++;

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

