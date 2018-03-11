#pragma once

#include <sol.hpp>
#include "../_entity/Entity.h"
#include <Manager.h>


/**
Factory class that has the capability to instantiate an Entity based on
a Lua script and add all of its necessary components.

It holds two maps containing the supported components for every entity and
the function necessary to initialize each of those components from the
data read from the Lua script.

@see Entity
*/
class EntityManager : public Manager<EntityManager> {
    using OwningEntitiesGroup = std::unordered_map<EntityId, std::unique_ptr<Entity>>;
private:
    friend class CRSP <EntityManager>;
    EntityManager();
    ~EntityManager();

public:

    void startUp() override;
    void shutDown() override;


    Entity* loadEntity(const sol::table& table, const std::string& name);

    bool hasEntity(EntityId id) const;

    Entity* getEntity(EntityId id);

private:

    void exposeToLua();

    OwningEntitiesGroup m_topEntities{};
    EntityId m_nextId{ 0 };

};

