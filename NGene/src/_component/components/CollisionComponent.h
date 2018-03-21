#pragma once
#include "../ComponentTemplate.h"

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

    /*
    Go to ComponentManager.h and register the name and constructor
    of this component so it can be instantiated.
    */


    /*
    When adding a member reMember to consider registering it
    in "expose_to_lua()" and in "meta::registerMembers" as well
    as add it in "drawComponentInspector()" to be able to see it
    */
    // ====== BEG OF MEMBERS ======

    sf::Vector2f m_offset{ 0.0f,0.0f };
    sf::Vector2f m_extent{ 10.0f,10.0f };
    bool m_dynamic{ false };
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
        member("dynamic", &CollisionComponent::m_dynamic)
    );
}


