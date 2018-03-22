#include "InputSystem.h"
#include <../_component/components/InputComponent.h>
#include <../time/TimeManager.h>
#include "../window/WindowManager.h"
#include "../input/InputManager.h"

InputSystem::InputSystem() {}


InputSystem::~InputSystem() {}

void InputSystem::start_up() {

    /*
    We register the system so we can receive the entities with the components we are interested in.
    */
    registerSystem();
}

void InputSystem::shut_down() {

}



void InputSystem::update() {

    const auto& events = WindowManager::get().get_frame_events();

    for (const auto& event : events) {
        for (auto& entity : m_entities) {
            auto input = entity.second->get_component<InputComponent>();

            switch (event.type) {

            case sf::Event::KeyPressed: {
                if (!m_keys_pressed[event.key.code]
                    && input->m_on_key_down)
                    input->m_on_key_down(entity.second, event.key.code);
                break;
            }
            case sf::Event::KeyReleased: {
                if (m_keys_pressed[event.key.code]
                    && input->m_on_key_up)
                    input->m_on_key_up(entity.second, event.key.code);
                break;
            }


            case sf::Event::JoystickButtonPressed: {
                if (!m_buttons_pressed[event.joystickButton.button]
                    && input->m_on_button_down)
                    input->m_on_button_down(entity.second, event.joystickButton.button);
                break;
            }
            case sf::Event::JoystickButtonReleased: {
                if (m_buttons_pressed[event.joystickButton.button]
                    && input->m_on_button_up)
                    input->m_on_button_up(entity.second, event.joystickButton.button);
                break;
            }

                                                    /*
                                                    We can use case sf::Event::JoystickMoved if we want to
                                                    get the event of just moving each joystick, which is kind
                                                    of weird since for that we just tend to poll for its state
                                                    constantly and not only when it changes
                                                    */

            }
        }

        /*
        It's preferable to do this so we can retrieve only once the entity data
        */
        switch (event.type) {
        case sf::Event::KeyPressed:
            m_keys_pressed[event.key.code] = true;
            break;
        case sf::Event::KeyReleased:
            m_keys_pressed[event.key.code] = false;
            break;
        case sf::Event::JoystickButtonPressed:
            m_buttons_pressed[event.joystickButton.button] = true;
            break;
        case sf::Event::JoystickButtonReleased:
            m_buttons_pressed[event.joystickButton.button] = false;
            break;
        }
    }

    float x = InputManager::get().get_axis_position(sf::Joystick::Axis::X);
    float y = InputManager::get().get_axis_position(sf::Joystick::Axis::Y);

    float u = InputManager::get().get_axis_position(sf::Joystick::Axis::U);
    float r = InputManager::get().get_axis_position(sf::Joystick::Axis::R);

    for (auto& entity : m_entities) {
        auto input = entity.second->get_component<InputComponent>();

        if (input->m_for_left_joystick)
            input->m_for_left_joystick(
            (entity.second),
                (abs(x) > input->m_joystick_threshold ? x : 0.0f),
                (abs(y) > input->m_joystick_threshold ? y : 0.0f));

        if (input->m_for_right_joystick)
            input->m_for_right_joystick(
            (entity.second),
                (abs(u) > input->m_joystick_threshold ? u : 0.0f),
                (abs(r) > input->m_joystick_threshold ? r : 0.0f));

    }


}


void InputSystem::register_entity(Entity& entity) {
    if (entity.has_component<InputComponent>()) {
        m_entities[entity.get_id()] = &entity;
    }
}

void InputSystem::deregister_entity(EntityId id) {
    m_entities.erase(id);
}


