#pragma once
#include "../System.h"
#include <map>

class TransformSystem : public System<TransformSystem> {
    using EntitiesGroup = std::map<EntityId, Entity*>;
    friend class CRSP<TransformSystem>;
private:
    TransformSystem();
    ~TransformSystem();
public:

    void update() override;
    void start_up() override;
    void shut_down() override;
    void register_entity(Entity& entity) override;
    void deregister_entity(EntityId id) override;

private:
    EntitiesGroup m_entities;
};

