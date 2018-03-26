#pragma once
#include "../System.h"
#include <map>
#include <memory>
#include <set>
#include "../types/def.h"
#include <../physics/ChunkManager.h>


class CollisionSystem : public System<CollisionSystem> {
    using EntitiesGroup = std::map<EntityId, Entity*>;
    friend class CRSP<CollisionSystem>;
private:
    CollisionSystem();
    ~CollisionSystem();
public:

    void update() override;
    void start_up() override;
    void shut_down() override;
    void register_entity(Entity& entity) override;
    void deregister_entity(EntityId id) override;

    std::set<Entity*> check_entity_set(const std::set<Entity*>& set);

    std::set<Entity*> recheck_entity_set(const std::set<Entity*>& set_to_check, const ChunkManager::GroupedEntities& all_entities);


private:

    bool are_static_entities(Entity*& entity_a, Entity*& entity_b);

    bool check_and_sort_types(Entity*& entity_a, Entity*& entity_b, ColliderType type_a, ColliderType type_b);

    void execute_collisions(Entity* entity_a, Entity* entity_b, std::set<Entity*>& to_recheck, bool rechecking = false);
    
    EntitiesGroup m_entities;
    std::unordered_set<std::pair<EntityId, EntityId>, PairIntIntHash> m_already_checked{};
};

