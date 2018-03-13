#include "PhraseComponent.h"
#include "../lua/LuaManager.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

PhraseComponent::PhraseComponent(EntityId id) : Component(id, std::type_index(typeid(PhraseComponent))) {
}

PhraseComponent::PhraseComponent(EntityId id, const sol::table& table)
    : Component(id, std::type_index(typeid(PhraseComponent))) {

    meta::doForAllMembers<PhraseComponent>([this, &table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.getName();
        sol::object value_obj = table[name];
        assert(value_obj.valid());
        auto value = value_obj.as<std::string>();
        member.set(*this, value);
    });

}

PhraseComponent::~PhraseComponent(){
}


void PhraseComponent::drawDebugGUI() {
    return Component::drawBullet<PhraseComponent>(m_parentId);
}

void PhraseComponent::drawComponentInspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculateShowname<PhraseComponent>(m_parentId).c_str(), &m_guiOpen);

    ImGui::Text("Phrase: %s", m_phrase.c_str());

    /*
    //To Check the window size and adjust default sizes:
    ImGui::Text("Window Size: (%f, %f)", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    */
    ImGui::End();

}

#define REGISTER_METHOD(method) #method , &PhraseComponent::method


void PhraseComponent::exposeToLua()
{

    LUA.new_usertype<PhraseComponent>("PhraseComponent",


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "phrase", sol::property(&PhraseComponent::getPhrase, &PhraseComponent::setPhrase)

        );

}


std::string PhraseComponent::getPhrase() const {
    return m_phrase;
}


void PhraseComponent::setPhrase(const std::string& phrase) {
    m_phrase = phrase;
}