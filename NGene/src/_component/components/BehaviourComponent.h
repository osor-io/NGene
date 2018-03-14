#pragma once
#include "../ComponentTemplate.h"
#include <sol.hpp>

class BehaviourComponent : public ComponentTemplate<BehaviourComponent> {
    friend auto meta::registerMembers<BehaviourComponent>();
public:
    BehaviourComponent(EntityId id);
    BehaviourComponent(EntityId id, const sol::table& table);
    ~BehaviourComponent();

    json toJson() override;
    void loadJson(const json& j) override;

    sol::function getOnUpdate() const;
    void setOnUpdate(const sol::function& member); // Use const& when appropriate

    std::string getOnUpdateString() const;
    void setOnUpdateString(const std::string& onUpdateString);

    // ====== BEG OF REQUIREMENTS ======
    void drawComponentInspector() override;
    static void exposeToLua();  // Go To Entity.cpp and call this
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
    sol::function m_onUpdate;
    std::string m_onUpdateString;
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


