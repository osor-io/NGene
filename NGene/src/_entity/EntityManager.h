#pragma once

#include <sol.hpp>
#include "../_entity/Entity.h"
#include <Manager.h>
#include <map>
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

    void start_up() override;
    void shut_down() override;

    void update_entities();

    EntityId request_load_entity(const std::string & type, const sol::table & table= LUA["Entities"]);

    void remove_entity(EntityId id);

    bool has_entity(EntityId id) const;

    Entity* get_entity(EntityId id);

    size_t entity_size() const;

    std::vector<EntityId> get_entity_keys() const;

    OwningEntitiesGroup& get_entities();

    json serialize_entities() const;

    void serialize_entities_to_file(const char* filename) const;

    void clear_and_load_entities_from_file(const char* filename);

    void clear_entities();

    void clear_and_load_entities(const json& j);

private:

    void expose_to_lua();

    OwningEntitiesGroup m_entities{};
    PendingEntitiesToAdd m_entities_to_add{};
    PendingEntitiesToDelete m_entity_ids_to_remove{};

    EntityId m_next_id{ 0 };

    bool m_requested_clear{ false };
    bool m_requested_load{ false };

    json m_unprocessed_load_entity_data{};

    Entity* create_entity_internal(const std::string & type, const sol::table & table = LUA["Entities"]);

};

