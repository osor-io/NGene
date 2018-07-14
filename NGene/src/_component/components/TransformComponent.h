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
#include <SFML\Graphics.hpp>
#include "../lua/LuaTypes.h"

/**

Component that represents the position of an entity in the world.
If an entity has this it means that it can be "located" in the world, this 
includes entities like characters, maps, gameplay objects, etc.

*/
class TransformComponent : public ComponentTemplate<TransformComponent> {
public:
    TransformComponent(EntityId id);
    TransformComponent(EntityId id, const sol::table& table);
    ~TransformComponent();

    json to_json() override;
    void load_json(const json& j) override;


    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua();
    friend auto meta::registerMembers<TransformComponent>();
    // ====== END OF REQUIREMENTS ======

    sf::Vector2f get_position() const;
    sf::Vector2f& get_position_ref();
    void set_position(const sf::Vector2f& position);

    LuaVector2f& lua_get_position();
    void lua_set_position(const LuaVector2f& position);



	
    // ====== BEG OF MEMBERS ======
    
    /**
    
    Represents the current position of the entity in the world.
    This is the attribute that will be changed during the generation
    of each frame.

    */
    sf::Vector2f m_position;
    
    /**
    
    Stores the previous position of the entity before all the changes that
    have been performed during the current frame. It us usefull to calculate
    things like its vector movement or to come back to a valid position after a 
    collision has happened with the new position.

    */
    sf::Vector2f m_previous_position;
    
    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<TransformComponent>() {
    return "TransformComponent";
}

template<>
inline auto meta::registerMembers<TransformComponent>() {
    return members(
        member("position", &TransformComponent::m_position)
    );
}

