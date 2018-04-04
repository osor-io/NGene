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

Component used as an example to copy and paste this and its
implementation file to easily create a new component that is fully
integrated with the rest of the engine and functional.

IMPORTANT: We need to update this documentation when we create a new
component as well as document the relevant members that the new component
will have.

@see ExampleComponent.cpp

*/
class ExampleComponent : public ComponentTemplate<ExampleComponent> {
    friend auto meta::registerMembers<ExampleComponent>();
public:
    ExampleComponent(EntityId id);
    ExampleComponent(EntityId id, const sol::table& table);
    ~ExampleComponent();

    json to_json() override;
    void load_json(const json& j) override;

    int get_member() const;
    void set_member(int member); // Use const& when appropriate

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
    
    /**
    
    This is an example of the documentation of a possible member that one
    component might have.
    
    */
    int m_member;

    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<ExampleComponent>() {
    return "ExampleComponent";
}

template<>
inline auto meta::registerMembers<ExampleComponent>() {
    return members(
        member("member", &ExampleComponent::get_member, &ExampleComponent::set_member)
    );
}


