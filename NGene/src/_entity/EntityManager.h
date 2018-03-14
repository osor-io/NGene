#pragma once

#include <sol.hpp>
#include "../_entity/Entity.h"
#include <Manager.h>
#include <map>
#include <future>
#include "../lua/LuaManager.h"



/**
Factory class that has the capability to instantiate an Entity based on
a Lua script and add all of its necessary components.

It holds two maps containing the supported components for every entity and
the function necessary to initialize each of those components from the
data read from the Lua script.

@see Entity
*/
class EntityManager : public Manager<EntityManager> {
    using OwningEntitiesGroup = std::map<EntityId, std::unique_ptr<Entity>>;
    using PendingEntitiesToAdd = std::vector<Entity*>;
    using PendingEntitiesToDelete = std::vector<EntityId>;
private:
    friend class CRSP <EntityManager>;
    EntityManager();
    ~EntityManager();

public:

    void startUp() override;
    void shutDown() override;

    void updateEntities();

    EntityId loadEntity(const std::string & type, const sol::table & table= LUA["Entities"]);

    void removeEntity(EntityId id);

    bool hasEntity(EntityId id) const;

    Entity* getEntity(EntityId id);

    size_t numberOfEntities() const;

    std::vector<EntityId> getEntityKeys() const;

    json serializeEntities() const;

    void clearAndloadEntities(const json& j);

private:

    void exposeToLua();

    OwningEntitiesGroup m_entities{};
    PendingEntitiesToAdd m_entitiesToAdd{};
    PendingEntitiesToDelete m_entitieIdsToRemove{};

    EntityId m_nextId{ 0 };

    bool m_requestedClearAndLoad{ false };
    json m_loadEntityData{};

    Entity* createEntityInternal(const std::string & type, const sol::table & table = LUA["Entities"]);

};

