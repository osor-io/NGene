#include "CollisionSystem.h"
#include <../_component/components/CollisionComponent.h>
#include <../time/TimeManager.h>
#include <../physics/ChunkManager.h>


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
        /*
        There was no collision
        */
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

void CollisionSystem::update() {

    const auto& grouped_entities = ChunkManager::get().get_grouped_entities_with_components<TransformComponent, CollisionComponent>();


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

                    auto transform_component_b = entity_b->get_component<TransformComponent>();

                    auto collision_component_b = entity_b->get_component<CollisionComponent>();


                    if (!collision_component_a->m_dynamic && !collision_component_b->m_dynamic) {
                        /*

                        */
                        continue;
                    }
                    else if (collision_component_a->m_dynamic && !collision_component_b->m_dynamic ||
                        !collision_component_a->m_dynamic && collision_component_b->m_dynamic) {

                        //Swept AABB Collision Detection for a dynamic and static entity

                        auto dynamic_entity = (collision_component_a->m_dynamic ? entity_a : entity_b);
                        auto static_entity = (collision_component_a->m_dynamic ? entity_b : entity_a);

                        float normal_x, normal_y;
                        auto velocity = sf::Vector2f{};

                        const auto collision_time = calculate_swept_collision_time(
                            transform_component_a,
                            transform_component_b,
                            collision_component_a,
                            collision_component_b,
                            velocity,
                            normal_x, normal_y
                        );

                        auto transform_to_move = (collision_component_a->m_dynamic ? transform_component_a : transform_component_b);


                        LOG_NAMED(collision_time);
                        LOG_NAMED(normal_x);
                        LOG_NAMED(normal_y);
                        //LOG("Before: " << transform_to_move->m_position.x << ", " << transform_to_move->m_position.y);
                        //transform_to_move->m_position = transform_to_move->m_previous_position + ((transform_to_move->m_position - transform_to_move->m_previous_position) * collision_time);
                        //LOG("After: " << transform_to_move->m_position.x << ", " << transform_to_move->m_position.y);


                        /*
                        auto remaining_time = 1.0f - collision_time;
                        auto dot_product = (transform_to_move->m_position.x * normal_x + transform_to_move->m_position.y * normal_y) * remaining_time;
                        transform_to_move->m_position.x = dot_product * normal_y;
                        transform_to_move->m_position.y = dot_product * normal_x;
                        */
                    }
                    else {


                        auto center_a = transform_component_a->get_position_ref() + collision_component_a->m_offset;
                        auto center_b = transform_component_b->get_position_ref() + collision_component_b->m_offset;

                        auto half_size_a = collision_component_a->m_extent;
                        auto half_size_b = collision_component_b->m_extent;

                        auto velocity_a = transform_component_a->m_position - transform_component_a->m_previous_position;
                        auto velocity_b = transform_component_b->m_position - transform_component_b->m_previous_position;


                        // Traditional AABB SAT collision testing
                        //@@TODO: Do swept and/or apply correction vectors to the transform
                        if (fabs(center_a.x - center_b.x) < (half_size_a.x + half_size_b.x) &&
                            fabs(center_a.y - center_b.y) < (half_size_a.y + half_size_b.y)) {
                            LOG("They collide");
                        }


                    }
                }
                
                //transform_component_a->m_position = new_position;

            }



        }
    }


}


void CollisionSystem::register_entity(Entity& entity) {}

void CollisionSystem::deregister_entity(EntityId id) {}


