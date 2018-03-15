#pragma once
#include "../ComponentTemplate.h"
#include <sol.hpp>

class BehaviourComponent : public ComponentTemplate<BehaviourComponent> {
    friend auto meta::registerMembers<BehaviourComponent>();
public:
    BehaviourComponent(EntityId id);
    BehaviourComponent(EntityId id, const sol::table& table);
    ~BehaviourComponent();

    json to_json() override;
    void load_json(const json& j) override;

    sol::function get_on_update() const;
    void set_on_update(const sol::function& member); // Use const& when appropriate

    std::string get_on_update_string() const;
    void set_on_update_string(const std::string& onUpdateString);

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
    sol::function m_on_update;
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


