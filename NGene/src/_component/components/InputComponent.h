#pragma once
#include "../ComponentTemplate.h"
#include "SFML/Window.hpp"

class InputComponent : public ComponentTemplate<InputComponent> {
    friend auto meta::registerMembers<InputComponent>();
public:
    InputComponent(EntityId id);
    InputComponent(EntityId id, const sol::table& table);
    ~InputComponent();

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
    std::function<void(Entity* entity, sf::Keyboard::Key key)> m_on_key_up;
    std::function<void(Entity* entity, sf::Keyboard::Key key)> m_on_key_down;
    std::function<void(Entity* entity, unsigned int button)> m_on_button_up;
    std::function<void(Entity* entity, unsigned int button)> m_on_button_down;
    std::function<void(Entity* entity, float x, float y)> m_for_left_joystick;
    std::function<void(Entity* entity, float x, float y)> m_for_right_joystick;
    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<InputComponent>() {
    return "InputComponent";
}

template<>
inline auto meta::registerMembers<InputComponent>() {
    return members(
        member("onKeyUp", &InputComponent::m_on_key_up),
        member("onKeyDown", &InputComponent::m_on_key_down),
        member("onButtonUp", &InputComponent::m_on_button_up),
        member("onButtonDown", &InputComponent::m_on_button_down),
        member("forLeftJoystick", &InputComponent::m_for_left_joystick),
        member("forRightJoystick", &InputComponent::m_for_right_joystick)
    );
}


