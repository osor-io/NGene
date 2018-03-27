#pragma once
#include "../ComponentTemplate.h"

enum class ColliderType {
    TERRAIN,
    MOVING_OBJECT,
};

enum CollisionDirectionFlags {
    COLLISION_DIRECTION_DOWN = 1,
    COLLISION_DIRECTION_UP = (1 << 1),
    COLLISION_DIRECTION_LEFT = (1 << 2),
    COLLISION_DIRECTION_RIGHT = (1 << 3),
};

class CollisionComponent : public ComponentTemplate<CollisionComponent> {
    friend auto meta::registerMembers<CollisionComponent>();
public:
    CollisionComponent(EntityId id);
    CollisionComponent(EntityId id, const sol::table& table);
    ~CollisionComponent();

    json to_json() override;
    void load_json(const json& j) override;


    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua();  // Go To Entity.cpp and call this
    // ====== END OF REQUIREMENTS ======

    void draw_rect();



    // ====== BEG OF MEMBERS ======

    sf::Vector2f m_offset{ 0.0f,0.0f };
    sf::Vector2f m_extent{ 10.0f,10.0f };


    /*

    Here it is a good article outlining some of the interesting types of collisions
    to support, I'm thinking about prioritizing Moving Plattforms and One Way Plattforms:

    http://higherorderfun.com/blog/2012/05/20/the-guide-to-implementing-2d-platformers/

    */
    ColliderType m_type{ ColliderType::TERRAIN };

    /*
    These are the flags set for the moving entities that containg which directions
    have been hit in relation to Terraing-like colliders, this is especially usefull to know
    if an entity is grounded or sliding on a wall.
    */
    unsigned int m_moving_collision_direction_flags{ 0u };

    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<CollisionComponent>() {
    return "CollisionComponent";
}

template<>
inline auto meta::registerMembers<CollisionComponent>() {
    return members(
        member("extent", &CollisionComponent::m_extent),
        member("offset", &CollisionComponent::m_offset),
        member("type", &CollisionComponent::m_type)
    );
}


