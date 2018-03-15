#include "BehaviourComponent.h"
#include "../lua/LuaManager.h"
#include <sstream>

#define COMPONENT_TYPE BehaviourComponent
#define CTOR(x) x##::##x
#define DTOR(x) x##::##~##x
#define STRINGIFY(s) #s

CTOR(COMPONENT_TYPE)(EntityId id) : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {}

CTOR(COMPONENT_TYPE)(EntityId id, const sol::table& table)
    : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {

    sol::function f = table["onUpdate"];

    m_on_update = f;

    auto ss = std::stringstream{};
    ss << "Valid: " << f.valid() << " Registry Index: " <<  f.registry_index();
    m_on_update_string = ss.str();
}


DTOR(COMPONENT_TYPE)() {}


json COMPONENT_TYPE::to_json() {
    return json{};
}

void COMPONENT_TYPE::load_json(const json& j) {
}

sol::function COMPONENT_TYPE::get_on_update() const {
    return m_on_update;
}


void COMPONENT_TYPE::set_on_update(const sol::function& f) {
    m_on_update = f;
}

std::string COMPONENT_TYPE::get_on_update_string() const {
    return m_on_update_string;
}


void COMPONENT_TYPE::set_on_update_string(const std::string& onUpdateString) {
    m_on_update_string = onUpdateString;
}


void COMPONENT_TYPE::draw_component_inspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculate_showname().c_str(), &m_gui_open);

    ImGui::Text("%s", m_on_update_string.c_str());

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

        "onUpdate", sol::property(&COMPONENT_TYPE::get_on_update, &COMPONENT_TYPE::set_on_update)

        );

}