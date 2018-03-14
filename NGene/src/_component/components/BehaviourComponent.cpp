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

    m_onUpdate = f;

    auto ss = std::stringstream{};
    ss << "Valid: " << f.valid() << " Registry Index: " <<  f.registry_index();
    m_onUpdateString = ss.str();
}


DTOR(COMPONENT_TYPE)() {}


json COMPONENT_TYPE::toJson() {
    return json{};
}

void COMPONENT_TYPE::loadJson(const json& j) {
}

sol::function COMPONENT_TYPE::getOnUpdate() const {
    return m_onUpdate;
}


void COMPONENT_TYPE::setOnUpdate(const sol::function& f) {
    m_onUpdate = f;
}

std::string COMPONENT_TYPE::getOnUpdateString() const {
    return m_onUpdateString;
}


void COMPONENT_TYPE::setOnUpdateString(const std::string& onUpdateString) {
    m_onUpdateString = onUpdateString;
}


void COMPONENT_TYPE::drawComponentInspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculateShowname().c_str(), &m_guiOpen);

    ImGui::Text("%s", m_onUpdateString.c_str());

    /*
    //To Check the window size and adjust default sizes:
    ImGui::Text("Window Size: (%f, %f)", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    */
    ImGui::End();

}

#define REGISTER_METHOD(method) #method , &PhraseComponent::method

void COMPONENT_TYPE::exposeToLua()
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

        "onUpdate", sol::property(&COMPONENT_TYPE::getOnUpdate, &COMPONENT_TYPE::setOnUpdate)

        );

}