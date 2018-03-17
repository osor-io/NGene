#include "InputComponent.h"
#include "../lua/LuaManager.h"

#define COMPONENT_TYPE InputComponent
#define CTOR(x) x##::##x
#define DTOR(x) x##::##~##x
#define STRINGIFY(s) #s

CTOR(COMPONENT_TYPE)(EntityId id) : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {}

CTOR(COMPONENT_TYPE)(EntityId id, const sol::table& table)
    : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {


    meta::doForAllMembers<InputComponent>([this, &table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.get_name();
        sol::object value_obj = table[name];
        if (value_obj.valid()) {
            auto value = value_obj.as<MemberT>();
            member.set(*this, value);
        }
    });

}


DTOR(COMPONENT_TYPE)() {
}


json COMPONENT_TYPE::to_json() {
    auto j = json{};
    j["joystickThreshold"] = m_joystick_threshold;
    return j;
}

void COMPONENT_TYPE::load_json(const json& j) {
    m_joystick_threshold = j["joystickThreshold"];
}


void COMPONENT_TYPE::draw_component_inspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculate_showname().c_str(), &m_gui_open);

    auto count = int{};

    ImGui::Text("Joystick Threshold: "); ImGui::SameLine(150); ImGui::DragFloat("##Threshold", &m_joystick_threshold, 1.0f, 0.0f, 100.0f);

    /*
    // To print which functions are implemented
    meta::doForAllMembers<InputComponent>([this, &count](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.get_name();
        if (member.get(*this)) {
            ImGui::TextWrapped("%s: REGISTERED\n\t%s", name, member.get(*this).target_type().name());
        }
        else {
            ImGui::TextWrapped("%s: NOT REGISTERED", name);
        }
    });
    */

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


        "onKeyUp", &InputComponent::m_on_key_up,
        "onKeyDown", &InputComponent::m_on_key_down,
        "onButtonUp", &InputComponent::m_on_button_up,
        "onButtonDown", &InputComponent::m_on_button_down,
        "forLeftJoystick", &InputComponent::m_for_left_joystick,
        "forRightJoystick", &InputComponent::m_for_right_joystick


        );

}