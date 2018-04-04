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
#include "SFML/Window.hpp"

/**

Component that represents the behaviour that the entity should
have when certain inputs from the player happen. This refers generally
to controller or mouse&keyboard inputs.

*/
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
    static void expose_to_lua(); 
    // ====== END OF REQUIREMENTS ======


    // ====== BEG OF MEMBERS ======
    
    /**
    
    This first set of functions will be documented with this general block
    since their functionality is pretty much the same. The only difference
    is wether they respond to keys, buttons, in which action (press or stop
    pressing).

    They store the function that gets executed when the event defined in their
    name happens, e.g. m_on_key_down will get executed whenever a key is pressed.

    The arguments will be the entity that contains this component and which button
    or key has been pressed or released.

    The functions in LUA whould have the form:

        onKeyUp = function (this, key)
            if(button == Key.A) then
                --We do something because the A key was pressed
            else
                --We could check for some other key or just do nothing
            end
        end,
    
    */

    /**
    @see InputComponent.h
    */
    std::function<void(Entity* entity, sf::Keyboard::Key key)> m_on_key_up;

    /**
    @see InputComponent.h
    */
    std::function<void(Entity* entity, sf::Keyboard::Key key)> m_on_key_down;

    /**
    @see InputComponent.h
    */
    std::function<void(Entity* entity, unsigned int button)> m_on_button_up;
    
    /**
    @see InputComponent.h
    */
    std::function<void(Entity* entity, unsigned int button)> m_on_button_down;

    /**
    
    This functions are called each frame and tell the LUA script the 
    state of each joystick so they can update accordingly, the functions could
    have this form (in this example we are moving the entity):

        forLeftJoystick = function (this, x, y)
            local deltaTime = getDeltaTime()
            transform = this:getTransformComponent()
            if(transform) then
                transform.position.x = transform.position.x + (x * 3 * deltaTime)
                transform.position.y = transform.position.y + (y * 3 * deltaTime)
            end
        end,

    */

    /**
    @see InputComponent.h
    */
    std::function<void(Entity* entity, float x, float y)> m_for_left_joystick;

    /**
    @see InputComponent.h
    */
    std::function<void(Entity* entity, float x, float y)> m_for_right_joystick;

    /**
    
    This represents the threshold for the joysticks after which we consider there
    has been some sort of movement. This means that if the joystick has only moved
    slightly we will still send zero movement to the joystick functions above to avoid
    undesired behaviour.

    This is necessary because modern joysticks have the tendency to not be perfectly
    centered when the used is not touching them, sending non (0,0) positions and
    generating behaviours like movement when the player is not touching the controller.

    */
    float m_joystick_threshold{ 15.0f };
    
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
        member("forRightJoystick", &InputComponent::m_for_right_joystick),
        member("joystickThreshold", &InputComponent::m_joystick_threshold)
    );
}


