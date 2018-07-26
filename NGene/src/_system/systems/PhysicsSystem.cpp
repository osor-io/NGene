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
        
        platformer_component->m_grounded = ((collision_component->m_moving_collision_direction_flags & CollisionDirectionFlags::COLLISION_DIRECTION_DOWN) != 0u);
        
        if (platformer_component->m_need_recalculation) {
            
            platformer_component->m_need_recalculation = false;
            
            platformer_component->m_initial_jump_velocity.y =
                (2.f * platformer_component->m_jump_height * platformer_component->m_max_foot_speed)
                /
                (platformer_component->m_horizontal_distance_to_peak);
            
            platformer_component->m_our_gravity.y =
                (-2.f * platformer_component->m_jump_height * platformer_component->m_max_foot_speed * platformer_component->m_max_foot_speed)
                /
                (platformer_component->m_horizontal_distance_to_peak * platformer_component->m_horizontal_distance_to_peak);
            
            auto scaled_gravity = (platformer_component->m_our_gravity.y * platformer_component->m_downwards_gravity_scale);
            
            /*
            We are limiting the falling velocity to the velocity that then entity
            has when jumping and landing from an in-place jump where the initial Y
            position is the same as the final Y position.
            */
            platformer_component->m_max_velocity.y = abs(
                scaled_gravity
                *
                sqrt(-platformer_component->m_jump_height / (scaled_gravity / 2.f)));
            
        }
        
        auto acceleration = sf::Vector2f(0.0f, 0.0f);
        
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
            
            if (platformer_component->m_current_velocity.y > 0.f) {
                // Going up
                acceleration += platformer_component->m_our_gravity;
            }
            else {
                // Going Down
                acceleration += platformer_component->m_our_gravity * platformer_component->m_downwards_gravity_scale;
            }
            
        }
        
        /*
        @@NOTE
        
        Using Verlet Integration to calculate new velocity and position.
        This seems to be more than good enough for our purposes, that is,
 better than Euler integration (which is inconsistent with variable
 delta times) but not as expensive as Runge-Kutta methods while being
        quite good still.
        
        @see: https://en.wikipedia.org/wiki/Euler_method
        @see: https://en.wikipedia.org/wiki/Verlet_integration
        @see: https://gafferongames.com/post/integration_basics/
        @see: https://www.av8n.com/physics/symplectic-integrator.htm
        
        */
        
        // Update the current velocity given the acceleration
        platformer_component->m_current_velocity += acceleration * delta_time;
        
        // Restrict the Y component of the velocity to the max velocity
        // to limit the speed at which the entity can fall
        platformer_component->m_current_velocity.y =
            (abs(platformer_component->m_current_velocity.y) < platformer_component->m_max_velocity.y ?
             platformer_component->m_current_velocity.y :
             (platformer_component->m_current_velocity.y > 0 ?
              platformer_component->m_max_velocity.y :
              -platformer_component->m_max_velocity.y));
        
        
        //
        // @@NOTE: Y is being flipped because SFML's Y coordinate
        // grows when going down (The origin point is top-left) which means
        // that a negative Y velocity would mean that the entity is going
        // up. For our logic I prefer to have a negative velocity that means
        // something is going down.
        //
        auto applied_velocity = platformer_component->m_current_velocity;
        applied_velocity.y = -applied_velocity.y;
        acceleration.y = -acceleration.y;
        
        //
        // Finally we use Verlet Integration to calculate the position at
        // which the entity should be based on the velocity and acceleration.
        //
        // This method is more numerically stable and time reversable. It's
        // error is also constrained to each iteration which means that we 
        // don't have to worry about it accumulating error in subsequent frames.
        //
        transform_component->m_position +=
            (applied_velocity * delta_time)
            +
            ((acceleration * delta_time * delta_time) / 2.f);
        
        
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


