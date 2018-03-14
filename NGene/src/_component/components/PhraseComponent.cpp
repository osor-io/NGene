#include "PhraseComponent.h"
#include "../lua/LuaManager.h"


PhraseComponent::PhraseComponent(EntityId id) : ComponentTemplate(id, std::type_index(typeid(PhraseComponent))) {
}

PhraseComponent::PhraseComponent(EntityId id, const sol::table& table)
    : ComponentTemplate(id, std::type_index(typeid(PhraseComponent))) {

    meta::doForAllMembers<PhraseComponent>([this, &table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.getName();
        sol::object value_obj = table[name];
        assert(value_obj.valid());
        auto value = value_obj.as<std::string>();
        member.set(*this, value);
    });

}

PhraseComponent::~PhraseComponent() {
}


json PhraseComponent::toJson() {
    auto j = json{};

    j["phrase"] = m_phrase;

    return j;
}

void PhraseComponent::loadJson(const json & j) {
    m_phrase = j["phrase"].get<std::string>();
}

std::string PhraseComponent::getPhrase() const {
    return m_phrase;
}


void PhraseComponent::setPhrase(const std::string& phrase) {
    m_phrase = phrase;
}


int phraseEditCallback(ImGuiTextEditCallbackData *data) {
    auto o = reinterpret_cast<PhraseComponent*>(data->UserData);
    o->setPhrase(std::string(data->Buf));
    return 0;
}

void PhraseComponent::drawComponentInspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculateShowname().c_str(), &m_guiOpen);


    char name_arr[config::max_phrase_size];

    strncpy(name_arr, m_phrase.c_str(), sizeof(name_arr));
    name_arr[sizeof(name_arr) - 1] = 0;


    ImGui::Text("Phrase: ");
    ImGui::SameLine(100);
    ImGui::InputText("##Phrase", name_arr, config::max_phrase_size, ImGuiInputTextFlags_CallbackAlways, phraseEditCallback, reinterpret_cast<void*>(this));

    /*
    //To Check the window size and adjust default sizes:
    ImGui::Text("Window Size: (%f, %f)", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    */
    ImGui::End();

}

#define REGISTER_METHOD(method) #method , &PhraseComponent::method

void PhraseComponent::exposeToLua()
{

    LUA.new_usertype<PhraseComponent>(meta::getName<PhraseComponent>(),


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
