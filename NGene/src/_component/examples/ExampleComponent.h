#pragma once
#include "../ComponentTemplate.h"

class ExampleComponent : public ComponentTemplate<ExampleComponent> {
    friend auto meta::registerMembers<ExampleComponent>();
public:
    ExampleComponent(EntityId id);
    ExampleComponent(EntityId id, const sol::table& table);
    ~ExampleComponent();

    json toJson() override;
    void loadJson(const json& j) override;

    int getMember() const;
    void setMember(int member); // Use const& when appropriate

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
        member("member", &ExampleComponent::getMember, &ExampleComponent::setMember)
    );
}


