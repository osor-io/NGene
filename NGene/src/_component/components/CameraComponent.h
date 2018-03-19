#pragma once
#include "../ComponentTemplate.h"

class CameraComponent : public ComponentTemplate<CameraComponent> {
    friend auto meta::registerMembers<CameraComponent>();
public:
    CameraComponent(EntityId id);
    CameraComponent(EntityId id, const sol::table& table);
    ~CameraComponent();

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
    bool m_main_camera{false};
    float m_zoom{ 1.0f };
    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<CameraComponent>() {
    return "CameraComponent";
}

template<>
inline auto meta::registerMembers<CameraComponent>() {
    return members(
        member("mainCamera", &CameraComponent::m_main_camera),
        member("zoom", &CameraComponent::m_zoom)
    );
}


