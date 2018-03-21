#include "CollisionSystem.h"
#include <../_component/components/CollisionComponent.h>
#include <../time/TimeManager.h>
#include <../physics/ChunkManager.h>
#include "../physics/AABB.h"
#include "../math/Vector.h"


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

/*
inline float calculate_swept_collision_time(
    TransformComponent*& transform_component_a,
    TransformComponent*& transform_component_b,
    CollisionComponent*& collision_component_a,
    CollisionComponent*& collision_component_b,
    sf::Vector2f velocity_a,
    float& normal_x, float& normal_y) {

    sf::Vector2f    center_a, center_b,
        half_size_a, half_size_b,
        velocity_b;

    if (collision_component_a->m_dynamic) {
        center_a = transform_component_a->get_position_ref() + collision_component_a->m_offset;
        center_b = transform_component_b->get_position_ref() + collision_component_b->m_offset;

        half_size_a = collision_component_a->m_extent;
        half_size_b = collision_component_b->m_extent;

        velocity_a = transform_component_a->m_position - transform_component_a->m_previous_position;
        velocity_b = transform_component_b->m_position - transform_component_b->m_previous_position;
    }
    else {
        center_b = transform_component_a->get_position_ref() + collision_component_a->m_offset;
        center_a = transform_component_b->get_position_ref() + collision_component_b->m_offset;

        half_size_b = collision_component_a->m_extent;
        half_size_a = collision_component_b->m_extent;

        velocity_b = transform_component_a->m_position - transform_component_a->m_previous_position;
        velocity_a = transform_component_b->m_position - transform_component_b->m_previous_position;
    }

    //@@DOING: Check why this fails and why the time is negative
    if (fabs(center_a.x - center_b.x) < (half_size_a.x + half_size_b.x) &&
        fabs(center_a.y - center_b.y) < (half_size_a.y + half_size_b.y)) {
        LOG("They collide");
    }

    float distance_near_side_x, distance_near_side_y;
    float distance_far_side_x, distance_far_side_y;

    if (velocity_a.x > 0.f) {
        distance_near_side_x = (center_b.x - half_size_b.x) - (center_a.x + half_size_a.x);
        distance_far_side_x = (center_b.x + half_size_b.x) - (center_a.x - half_size_a.x);
    }
    else {
        distance_near_side_x = (center_b.x + half_size_b.x) - (center_a.x - half_size_a.x);
        distance_far_side_x = (center_b.x - half_size_b.x) - (center_a.x + half_size_a.x);
    }

    if (velocity_a.y > 0.f) {
        distance_near_side_y = (center_b.y - half_size_b.y) - (center_a.y + half_size_a.y);
        distance_far_side_y = (center_b.y + half_size_b.y) - (center_a.y - half_size_a.y);
    }
    else {
        distance_near_side_y = (center_b.y + half_size_b.y) - (center_a.y - half_size_a.y);
        distance_far_side_y = (center_b.y - half_size_b.y) - (center_a.y + half_size_a.y);
    }

    float in_time_x, in_time_y;
    float out_time_x, out_time_y;

    if (velocity_a.x == 0) {
        in_time_x = -std::numeric_limits<float>::infinity();
        out_time_x = std::numeric_limits<float>::infinity();
    }
    else {
        in_time_x = distance_near_side_x / velocity_a.x;
        out_time_x = distance_far_side_x / velocity_a.x;
    }

    if (velocity_a.y == 0) {
        in_time_y = -std::numeric_limits<float>::infinity();
        out_time_y = std::numeric_limits<float>::infinity();
    }
    else {
        in_time_y = distance_near_side_y / velocity_a.y;
        out_time_y = distance_far_side_y / velocity_a.y;
    }

    auto entry_time = std::max(in_time_x, in_time_y);
    auto exit_time = std::min(out_time_x, out_time_y);


    if (entry_time > exit_time || in_time_x < 0.f && in_time_y < 0.f || in_time_x > 1.f || in_time_y > 1.f) {

        //There was no collision

        normal_x = 0.0f;
        normal_y = 0.0f;
        return 1.0f;
    }
    else {
        if (in_time_x > in_time_y) {

            if (distance_near_side_x < 0.f) {
                normal_x = 1.0f;
                normal_y = 0.0f;
            }
            else {
                normal_x = -1.0f;
                normal_y = 0.0f;
            }

        }
        else {
            if (distance_near_side_y < 0.f) {
                normal_x = 0.0f;
                normal_y = 1.0f;
            }
            else {
                normal_x = 0.0f;
                normal_y = -1.0f;
            }

        }
        return entry_time;
    }
}
*/

void CollisionSystem::update() {

    const auto& grouped_entities = ChunkManager::get().get_grouped_entities_with_components<TransformComponent, CollisionComponent>();

    m_already_checked.clear();

    for (const auto & v : grouped_entities) {
        if (v.second.size() > 1) {

            auto begin = v.second.begin();
            for (auto a = begin; a != v.second.end(); ++a) {
                auto entity_a = (*a);
                auto transform_component_a = entity_a->get_component<TransformComponent>();
                auto collision_component_a = entity_a->get_component<CollisionComponent>();
                auto new_position = sf::Vector2f{};

                for (auto b = std::next(a); b != v.second.end(); ++b) {

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

                    auto transform_component_b = entity_b->get_component<TransformComponent>();
                    auto collision_component_b = entity_b->get_component<CollisionComponent>();

                    auto velocity_a = transform_component_a->m_position - transform_component_a->m_previous_position;
                    auto velocity_b = transform_component_b->m_position - transform_component_b->m_previous_position;

                    if (!collision_component_a->m_dynamic && !collision_component_b->m_dynamic) {
                        continue;
                    }
                    else if (collision_component_a->m_dynamic && !collision_component_b->m_dynamic ||
                        !collision_component_a->m_dynamic && collision_component_b->m_dynamic) {

                        //Swept AABB Collision Detection for a dynamic and static entity

                        auto dynamic_entity = (collision_component_a->m_dynamic ? entity_a : entity_b);
                        auto static_entity = (collision_component_a->m_dynamic ? entity_b : entity_a);

                        transform_component_a = dynamic_entity->get_component<TransformComponent>();
                        transform_component_b = static_entity->get_component<TransformComponent>();

                        collision_component_a = dynamic_entity->get_component<CollisionComponent>();
                        collision_component_b = static_entity->get_component<CollisionComponent>();


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
                                auto norm = normalize(penetration_vector);
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

                            transform_component_a->m_position = new_position_a + velocity_a;
                        }
                        else {


                            auto relative_motion = velocity_a - velocity_b;

                            auto h = minkowski_difference.ray_intersection_fraction(sf::Vector2f(0.0f, 0.0f), relative_motion);


                            if (h < std::numeric_limits<float>::infinity()) {

                                /*
                                We know they will collide in this frame.
                                */

                                /*
                                Maybe we should get the minimum h and then calculate when we finish the inner loop for the entity.
                                */
                                auto new_position_a = transform_component_a->m_previous_position + velocity_a * h;
                                auto new_position_b = transform_component_b->m_previous_position + velocity_b * h;

                                auto norm = normalize(relative_motion);
                                auto tan = tangent(norm);
                                velocity_a = dot(velocity_a, tan) * tan;
                                velocity_b = dot(velocity_b, tan) * tan;

                                transform_component_a->m_position = new_position_a + velocity_a;
                                transform_component_b->m_position = new_position_b + velocity_b;
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

                        auto box_a = AABB(transform_component_a->m_position + collision_component_a->m_offset,
                            collision_component_a->m_extent);

                        auto box_b = AABB(transform_component_b->m_position + collision_component_b->m_offset,
                            collision_component_b->m_extent);

                        auto mink_diff = box_a - box_b;

                        auto collided = mink_diff.has_origin();
                        if (collided) {
                            auto correction_vector = std::get<0>(mink_diff.closest_point_on_bounds_to_point(sf::Vector2f(0.0f, 0.0f)));
                            transform_component_a->m_position -= correction_vector;
                        }
                    }
                }
            }
        }
    }
}


void CollisionSystem::register_entity(Entity& entity) {}

void CollisionSystem::deregister_entity(EntityId id) {}


