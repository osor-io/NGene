/**

The general documentation for the components will be in
the Component.h and CompomentTemplate.h respectively. Here you
can find the documentation of the general members that every component
should have such as the serialization methods (to and from Json),
the ones used to expose the component to LUA scripts or draw its inspector
for the developer mode.

@see ComponentTemplate.h
@see Component.h

*/

#pragma once
#include "../ComponentTemplate.h"

/**

Component used to represent a camera in the engine. it marks that
the current entity must work as a camera and sets some of the
parameters that are relevant for it such as wether it is the main
camera or not or if we are using any sort of zoom.

*/
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
    static void expose_to_lua();  
    // ====== END OF REQUIREMENTS ======



    
    // ====== BEG OF MEMBERS ======

    /**
    
    Marks wether this is the main camera during gameplay or not.
    That is, this is the main thing we must render as it is seeing
    the main image that is considered "the game"
    
    */
    bool m_main_camera{false};

    /**
    
    This is the zoom value of this camera, it is used to basically
    see things closer of from further away. I know, I'm explaining
    the concept of zoom. We all know what this means :)
    
    */
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


