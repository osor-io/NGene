#pragma once
#include "../ComponentTemplate.h"

class ExtentComponent : public ComponentTemplate<ExtentComponent> {
    friend auto meta::registerMembers<ExtentComponent>();
public:
    ExtentComponent(EntityId id);
    ExtentComponent(EntityId id, const sol::table& table);
    ~ExtentComponent();

    json to_json() override;
    void load_json(const json& j) override;


    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua();  // Go To Entity.cpp and call this
    // ====== END OF REQUIREMENTS ======

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
    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<ExtentComponent>() {
    return "ExtentComponent";
}

template<>
inline auto meta::registerMembers<ExtentComponent>() {
    return members(
        member("extent", &ExtentComponent::m_extent),
        member("offset", &ExtentComponent::m_offset)
    );
}


