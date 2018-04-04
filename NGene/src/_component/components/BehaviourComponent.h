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
#include <sol.hpp>

/**

Component Used to define and update the behaviour of one particular entity.
It contains on_X methods that get executed when X happens in the game. For example
on_update will be executed in each of the ticks of the game loop, that is, during
each generation of the next frame.

*/
class BehaviourComponent : public ComponentTemplate<BehaviourComponent> {
    friend auto meta::registerMembers<BehaviourComponent>();
public:
    BehaviourComponent(EntityId id);
    BehaviourComponent(EntityId id, const sol::table& table);
    ~BehaviourComponent();

    json to_json() override;
    void load_json(const json& j) override;

    sol::function get_on_update() const;
    void set_on_update(const sol::function& member);

    std::string get_on_update_string() const;
    void set_on_update_string(const std::string& onUpdateString);

    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua();
    // ====== END OF REQUIREMENTS ======

   
    // ====== BEG OF MEMBERS ======
    
    /**

    LUA function with the form:
        
        onUpdate = function (this, deltaTime)
        end
    
    That is called every frame. The "this" parameter refers
    to the entity that contains this component and the deltaTime
    is the time that has passed between this frame and the previous one.

    */
    sol::function m_on_update;

    /**

    Stores a brief description of the on_update method
    that specifies its situation in LUA. That it, wether it
    is a valid method, and its ID in the current LUA State.

    */
    std::string m_on_update_string;
    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<BehaviourComponent>() {
    return "BehaviourComponent";
}

template<>
inline auto meta::registerMembers<BehaviourComponent>() {
    return members();
}


