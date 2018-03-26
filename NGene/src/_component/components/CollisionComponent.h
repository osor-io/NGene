#pragma once
#include "../ComponentTemplate.h"

enum class ColliderType {
    TERRAIN,
    MOVING_OBJECT,
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
    @@TODO
    
    We should extend this argument to an enum supporting various types of colliders
    to support various types of collisions. Just something being static or dynamic 
    is not enough. Think about moving plattforms or one way plattforms (where you
    can go through them when going from bottom to top but not in reverse)

    Here it is a good article outlining some of the interesting types of collisions
    to support, I'm thinking about prioritizing Moving Plattforms and One Way Plattforms:

    http://higherorderfun.com/blog/2012/05/20/the-guide-to-implementing-2d-platformers/
    
    */
    ColliderType m_type{ ColliderType::TERRAIN };
    
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


