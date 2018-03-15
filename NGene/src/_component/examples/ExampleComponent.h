#pragma once
#include "../ComponentTemplate.h"

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
    in "exposeToLua()" and in "meta::registerMembers" as well
    as add it in "drawComponentInspector()" to be able to see it
    */
    // ====== BEG OF MEMBERS ======
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


