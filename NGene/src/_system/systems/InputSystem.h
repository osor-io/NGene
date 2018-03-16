#pragma once
#include "../System.h"
#include <map>
#include <memory>

class InputSystem : public System<InputSystem> {
    using EntitiesGroup = std::map<EntityId, Entity*>;
    using KeyPressedMap = std::map<sf::Keyboard::Key, bool>;
    using ButtonPressedMap = std::map<unsigned int, bool>;


    friend class CRSP<InputSystem>;
private:
    InputSystem();
    ~InputSystem();
public:

    void update() override;
    void start_up() override;
    void shut_down() override;
    void register_entity(Entity& entity) override;
    void deregister_entity(EntityId id) override;

private:
    EntitiesGroup m_entities;

    KeyPressedMap m_keys_pressed;
    ButtonPressedMap m_buttons_pressed;
};

