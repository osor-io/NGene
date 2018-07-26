#pragma once
#include "../System.h"
#include <map>
#include <memory>


namespace config {
	constexpr auto velocity_zero_threshold = 0.01f;
	constexpr auto max_timestep = 1.f/15.f;
}

class PhysicsSystem : public System<PhysicsSystem> {
    using EntitiesGroup = std::map<EntityId, Entity*>;
    friend class CRSP<PhysicsSystem>;
private:
    PhysicsSystem();
    ~PhysicsSystem();
public:

    void update() override;
    void start_up() override;
    void shut_down() override;
    void register_entity(Entity& entity) override;
    void deregister_entity(EntityId id) override;

private:
    EntitiesGroup m_entities;
};

