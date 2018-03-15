#include "ExampleComponent.h"
#include "../lua/LuaManager.h"

#define COMPONENT_TYPE ExampleComponent
#define CTOR(x) x##::##x
#define DTOR(x) x##::##~##x
#define STRINGIFY(s) #s

CTOR(COMPONENT_TYPE)(EntityId id) : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {}

CTOR(COMPONENT_TYPE)(EntityId id, const sol::table& table)
    : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {

    meta::doForAllMembers<COMPONENT_TYPE>([this, &table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.get_name();
        sol::object value_obj = table[name];
        assert(value_obj.valid());
        auto value = value_obj.as<MemberT>();
        member.set(*this, value);
    });

}


DTOR(COMPONENT_TYPE)() {
}


json COMPONENT_TYPE::to_json() {
    auto j = json{};

    j["member"] = m_member;

    return j;
}

void COMPONENT_TYPE::load_json(const json& j) {
    m_member = j["member"];
}

int COMPONENT_TYPE::get_member() const {
    return m_member;
}


void COMPONENT_TYPE::set_member(int member) {
    m_member = member;
}


void COMPONENT_TYPE::draw_component_inspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculate_showname().c_str(), &m_gui_open);

    ImGui::Text("Example Member (int): %d", m_member);

    /*
    //To Check the window size and adjust default sizes:
    ImGui::Text("Window Size: (%f, %f)", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    */
    ImGui::End();

}

#define REGISTER_METHOD(method) #method , &PhraseComponent::method

void COMPONENT_TYPE::expose_to_lua()
{

    LUA.new_usertype<COMPONENT_TYPE>(STRINGIFY(COMPONENT_TYPE),


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "member", sol::property(&COMPONENT_TYPE::get_member, &COMPONENT_TYPE::set_member)

        );

}