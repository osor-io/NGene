#pragma once
#include "../ComponentTemplate.h"
#include <SFML/System/Vector2.hpp>


enum INPUT_MOVEMENT_DIRECTION {
    INPUT_MOVEMENT_RIGHT,
    INPUT_MOVEMENT_LEFT,
    INPUT_MOVEMENT_NONE
};

class PlatformerPhysicsComponent : public ComponentTemplate<PlatformerPhysicsComponent> {
    friend auto meta::registerMembers<PlatformerPhysicsComponent>();
public:
    PlatformerPhysicsComponent(EntityId id);
    PlatformerPhysicsComponent(EntityId id, const sol::table& table);
    ~PlatformerPhysicsComponent();

    json to_json() override;
    void load_json(const json& j) override;


    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua();  // Go To Entity.cpp and call this
    // ====== END OF REQUIREMENTS ======

    void move(INPUT_MOVEMENT_DIRECTION direction);
    void jump();


    // ====== BEG OF MEMBERS ======

    float m_max_foot_speed;
    float m_horizontal_distance_to_peak;
    float m_jump_height;
    float m_downwards_gravity_scale;

    sf::Vector2f m_our_gravity;
    sf::Vector2f m_initial_jump_velocity;
    sf::Vector2f m_max_velocity;

    sf::Vector2f m_current_velocity;

    bool m_grounded{ false };

    bool m_requested_jump{ false };

    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<PlatformerPhysicsComponent>() {
    return "PlatformerPhysicsComponent";
}

template<>
inline auto meta::registerMembers<PlatformerPhysicsComponent>() {
    return members(
        member("maxFootSpeed", &PlatformerPhysicsComponent::m_max_foot_speed),
        member("distanceToPeak", &PlatformerPhysicsComponent::m_horizontal_distance_to_peak),
        member("jumpHeight", &PlatformerPhysicsComponent::m_jump_height),
        member("downwardsGravityScale", &PlatformerPhysicsComponent::m_downwards_gravity_scale),
        member("ourGravity", &PlatformerPhysicsComponent::m_our_gravity),
        member("initialJumpVelocity", &PlatformerPhysicsComponent::m_initial_jump_velocity),
        member("maxVelocity", &PlatformerPhysicsComponent::m_max_velocity),
        member("currentVelocity", &PlatformerPhysicsComponent::m_current_velocity),
        member("grounded", &PlatformerPhysicsComponent::m_grounded)
    );
}


