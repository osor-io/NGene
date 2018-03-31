#include "PhysicsSystem.h"
#include <../time/TimeManager.h>

#include <../_component/components/PlatformerPhysicsComponent.h>
#include <../_component/components/TransformComponent.h>
#include <../_component/components/CollisionComponent.h>


PhysicsSystem::PhysicsSystem() {}


PhysicsSystem::~PhysicsSystem() {}

void PhysicsSystem::start_up() {

    /*
    We register the system so we can receive the entities with the components we are interested in.
    */
    registerSystem();
}

void PhysicsSystem::shut_down() {

}



void PhysicsSystem::update() {


    auto delta_time = DELTA_SECONDS;

    for (auto& e : m_entities) {

        /*
        Useful resource for the jumps: https://www.youtube.com/watch?v=hG9SzQxaCm8&t=453s
        */

        auto entity = e.second;

        auto platformer_component = entity->get_component<PlatformerPhysicsComponent>();
        auto transform_component = entity->get_component<TransformComponent>();
        auto collision_component = entity->get_component<CollisionComponent>();

        platformer_component->m_grounded = (collision_component->m_moving_collision_direction_flags & CollisionDirectionFlags::COLLISION_DIRECTION_DOWN != 0u);

        {
            //Calculate initial jump velocity and gravity

            platformer_component->m_initial_jump_velocity.y = 
                (2.f * platformer_component->m_jump_height * platformer_component->m_max_foot_speed)
                /
                (platformer_component->m_distance_to_peak);

            platformer_component->m_our_gravity.y =
                (-2.f * platformer_component->m_jump_height * platformer_component->m_max_foot_speed * platformer_component->m_max_foot_speed)
                /
                (platformer_component->m_distance_to_peak * platformer_component->m_distance_to_peak);

        }

        if (platformer_component->m_grounded) {

            platformer_component->m_current_velocity.y = 0.f;

            /*
            We allow the entity to jump and we don't push it downwards since
            it is going to collide with terrain anyway. This way we avoid an
            extra collision check each frame we are grounded.
            */

            if (platformer_component->m_requested_jump) {
                platformer_component->m_current_velocity += platformer_component->m_initial_jump_velocity;
                platformer_component->m_requested_jump = false;
                platformer_component->m_grounded = false;
            }

        }
        else {

            /*
            If we are not grounded then we have to apply downwards gravity
            */

            platformer_component->m_current_velocity += platformer_component->m_our_gravity * delta_time;

        }

        auto applied_velocity = platformer_component->m_current_velocity;
        applied_velocity.y *= -1;
        transform_component->m_position = transform_component->m_position + applied_velocity * delta_time;

    }

}


void PhysicsSystem::register_entity(Entity& entity) {
    if (entity.has_component<PlatformerPhysicsComponent>() && entity.has_component<TransformComponent>() && entity.has_component<CollisionComponent>()) {
        m_entities[entity.get_id()] = &entity;
    }
}

void PhysicsSystem::deregister_entity(EntityId id) {
    m_entities.erase(id);
}


