#include "InputSystem.h"
#include <../_component/components/InputComponent.h>
#include <../time/TimeManager.h>
#include "../window/WindowManager.h"

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
        switch (event.type) {
        case sf::Event::KeyPressed: {

            if (!m_keys_pressed[event.key.code]) {
                for (auto& entity : m_entities) {
                    auto input = entity.second->get_component<InputComponent>();
                    if (input->m_on_key_down) input->m_on_key_down(event.key.code);
                    //@@DOING: Calling all the InputComponents functions (remember to expose the Key enum to LUA)
                }
            }
            m_keys_pressed[event.key.code] = true;
            break;
        }
        case sf::Event::KeyReleased: {
            break;
        }
        
        }
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


