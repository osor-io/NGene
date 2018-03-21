#pragma once
#include "../System.h"
#include <map>
#include <memory>
#include "../types/def.h"
#include <set>

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

    std::set<Entity*> check_entity_set(std::set<Entity*> set);

private:
    EntitiesGroup m_entities;
    std::unordered_set<std::pair<EntityId, EntityId>, PairIntIntHash> m_already_checked{};
};

