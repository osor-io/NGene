#include "CollisionSystem.h"
#include <../_component/components/CollisionComponent.h>
#include <../time/TimeManager.h>
#include "../physics/AABB.h"
#include "../math/Vector.h"

#define RECHECK_LIMIT 100

CollisionSystem::CollisionSystem() {}

CollisionSystem::~CollisionSystem() {}

void CollisionSystem::start_up() {

    /*
    We register the system so we can receive the entities with the components we are interested in.
    */
    registerSystem();
}

void CollisionSystem::shut_down() {

}

void CollisionSystem::register_entity(Entity& entity) {}

void CollisionSystem::deregister_entity(EntityId id) {}

void CollisionSystem::update() {

    const auto& grouped_entities = ChunkManager::get().get_grouped_entities_with_components<TransformComponent, CollisionComponent>();


    m_already_checked.clear();

    for (const auto & v : grouped_entities) {
        if (v.second.size() > 1) {

            auto remaining_to_recheck = check_entity_set(v.second);
            
            for (auto i = 0; (remaining_to_recheck.size() > 0 && i < RECHECK_LIMIT); ++i) {
                for (auto e : remaining_to_recheck) {
                    auto transform = e->get_component<TransformComponent>();
                    transform->m_previous_position = transform->m_position;
                }
                m_already_checked.clear();
                remaining_to_recheck = recheck_entity_set(remaining_to_recheck, grouped_entities);
            }
        }
    }
}

std::set<Entity*> CollisionSystem::check_entity_set(const std::set<Entity*>& set) {

    auto to_recheck = std::set<Entity*>{};

    auto begin = set.begin();
    auto end = set.end();

    for (auto a = begin; a != end; ++a) {
        auto entity_a = (*a);

        auto new_position = sf::Vector2f{};

        for (auto b = std::next(a); b != end; ++b) {

            auto entity_b = (*b);

            /*
            We save the entities checked per frame to avoid repeated checks
            with entities that are in the same groups of chunks.
            */
            auto id_pair = std::make_pair(entity_a->get_id(), entity_b->get_id());
            if (m_already_checked.find(id_pair) != m_already_checked.end()) {
                continue;
            }
            m_already_checked.insert(id_pair);

            execute_collisions(entity_a, entity_b, to_recheck);

        }
    }
    return to_recheck;
}

std::set<Entity*> CollisionSystem::recheck_entity_set(const std::set<Entity*>& set_to_check, const ChunkManager::GroupedEntities & all_entities) {

    auto to_recheck = std::set<Entity*>{};

    auto begin = set_to_check.begin();
    auto end = set_to_check.end();

    for (auto a = begin; a != end; ++a) {
        auto entity_a = (*a);

        auto chunks = ChunkManager::get().calculate_chunks(entity_a);

        auto new_position = sf::Vector2f{};

        for (auto & chunk : chunks) {

            if (all_entities.find(chunk) == all_entities.end()) continue;

            auto entities_in_chunk = all_entities.at(chunk);

            for (auto b = entities_in_chunk.begin(); b != entities_in_chunk.end(); ++b) {

                auto entity_b = (*b);

                if (entity_a->get_id() == entity_b->get_id()) continue;

                /*
                We save the entities checked per frame to avoid repeated checks
                with entities that are in the same groups of chunks.
                */
                auto id_pair = std::make_pair(entity_a->get_id(), entity_b->get_id());
                if (m_already_checked.find(id_pair) != m_already_checked.end()) {
                    continue;
                }
                m_already_checked.insert(id_pair);

                auto previous_size = to_recheck.size();
                execute_collisions(entity_a, entity_b, to_recheck, true);
            }
        }
    }
    return to_recheck;
}

void CollisionSystem::execute_collisions(Entity * entity_a, Entity * entity_b, std::set<Entity*>& to_recheck, bool rechecking) {

    auto collision_component_a = entity_a->get_component<CollisionComponent>();
    auto collision_component_b = entity_b->get_component<CollisionComponent>();

    if (!collision_component_a->m_dynamic && !collision_component_b->m_dynamic) {
        return;
    }
    else if (collision_component_a->m_dynamic && !collision_component_b->m_dynamic ||
        !collision_component_a->m_dynamic && collision_component_b->m_dynamic) {

        //Swept AABB Collision Detection for a dynamic and static entity

        auto dynamic_entity = (collision_component_a->m_dynamic ? entity_a : entity_b);
        auto static_entity = (collision_component_a->m_dynamic ? entity_b : entity_a);

        auto transform_component_a = dynamic_entity->get_component<TransformComponent>();
        auto transform_component_b = static_entity->get_component<TransformComponent>();

        collision_component_a = dynamic_entity->get_component<CollisionComponent>();
        collision_component_b = static_entity->get_component<CollisionComponent>();

        auto velocity_a = transform_component_a->m_position - transform_component_a->m_previous_position;
        auto velocity_b = transform_component_b->m_position - transform_component_b->m_previous_position;

        auto box_a = AABB(transform_component_a->m_previous_position + collision_component_a->m_offset,
            collision_component_a->m_extent);

        auto box_b = AABB(transform_component_b->m_previous_position + collision_component_b->m_offset,
            collision_component_b->m_extent);

        auto minkowski_difference = box_b - box_a;

        auto collided = minkowski_difference.has_origin();

        if (collided) {


            /*
            We were already colliding.
            Actually, we should only be exactly by the collider, touching it
            since the "else" branch makes it impossible to get into one of the colliders.
            */

            /*
            We get how much we are penetrating inside the collider.
            */
            auto penetration_vector = sf::Vector2f{};

            /*
            And the normal vector that points towards the direction we cannot move.
            */
            auto normal_vector = sf::Vector2f{};

            std::tie(penetration_vector, normal_vector) = minkowski_difference.closest_point_on_bounds_to_point(sf::Vector2f(0.0f, 0.0f));

            /*
            Our new position will start from a valid one, taking the previous position and
            adding the penetration vector so we are not colliding anymore in case we were
            inside the collider.
            */
            auto new_position_a = transform_component_a->m_previous_position + penetration_vector;


            /*
            Now in case that we have entered the collider (which shouldn't happen)
            we only allow velocities that don't move us towards the penetration vector
            to keep us from moving into it again just after solving it.
            */
            if (magnitude_squared(penetration_vector) > 0.0f) {
                auto norm = -normalize(penetration_vector);
                if (dot(velocity_a, norm) > 0) {
                    auto tan = tangent(norm);
                    velocity_a = dot(velocity_a, tan) * tan;
                }
            }
            /*
            In the same way, but this actually happens very frequently, if we
            are right by the collider and the penetration vector is (0,0) then
            we simply avoid moving in the same direction that the normal of the
            side we are colliding into pointing inwards.
            */
            else {
                if (dot(velocity_a, normal_vector) > 0) {
                    auto tan = tangent(normal_vector);
                    velocity_a = dot(velocity_a, tan) * tan;
                }
            }

            new_position_a += velocity_a;
            if (transform_component_a->m_position != new_position_a) {
                to_recheck.insert(entity_a);
                transform_component_a->m_position = new_position_a;
            }
        }
        else {

            auto relative_motion = velocity_a - velocity_b;

            auto h = float{};
            auto normal = sf::Vector2f{};

            std::tie(h, normal) = minkowski_difference.ray_intersection_fraction(sf::Vector2f(0.0f, 0.0f), relative_motion);


            if (h < std::numeric_limits<float>::infinity()) {

                /*
                We know they will collide in this frame.
                */

                /*
                Maybe we should get the minimum h and then calculate when we finish the inner loop for the entity.
                */
                auto new_position_a = transform_component_a->m_previous_position + velocity_a * h;

                /*
                We use the normal we retrieved instead of the normal of
                the relative motion here:

                auto normal = normalize(relative_motion);

                Since in this case we want to slide the entity that is moving
                against the static one, to do that we need to keep the
                component of the velocity that is not pushing us towards
                the obstacle.
                */
                auto tan = tangent(normal);
                velocity_a = dot(velocity_a, tan) * tan;


                new_position_a += velocity_a;
                if (transform_component_a->m_position != new_position_a) {
                    transform_component_a->m_position = new_position_a;
                    to_recheck.insert(entity_a);
                }
            }
            else {
                /*
                There is no collision between the two entities!
                */
            }

        }

    }
    else {

        /*
        Simple Discrete Minkowski Difference check.
        In the Minkowski difference of two shapes, we know for sure if those two shapes are colliding
        if and only if the origin (0,0) is inside that difference.

        Why? Because the Minkowski difference mathematically represents substracting every single point
        from one shape from all the other shapes of the other point (infinite points indeed). Then, we
        know that if two objects are colliding they for sure are sharing at least one point. If we do
        the difference, since they share one point if the are colliding, that point will substract from
        itself leaving the point (0,0) in the resulting shape, that is, the origin.

        So, basically, if the origin is in the difference, then there was an equal point in both shapes.
        Or even simpler, they are colliding.
        */

        auto transform_component_a = entity_a->get_component<TransformComponent>();
        auto transform_component_b = entity_b->get_component<TransformComponent>();

        auto box_a = AABB(transform_component_a->m_position + collision_component_a->m_offset,
            collision_component_a->m_extent);

        auto box_b = AABB(transform_component_b->m_position + collision_component_b->m_offset,
            collision_component_b->m_extent);

        auto minkowski_difference = box_b - box_a;

        auto collided = minkowski_difference.has_origin();

        if (collided) {
            auto correction_vector = std::get<0>(minkowski_difference.closest_point_on_bounds_to_point(sf::Vector2f(0.0f, 0.0f)));
            if (magnitude_squared(correction_vector) > 0) {
                transform_component_a->m_position += correction_vector / 2.0f;
                to_recheck.insert(entity_a);
                transform_component_b->m_position -= correction_vector / 2.0f;
                to_recheck.insert(entity_b);
            }
        }
    }


}


