#include "PhraseComponent.h"
#include "../lua/LuaManager.h"


PhraseComponent::PhraseComponent(EntityId id) : ComponentTemplate(id, std::type_index(typeid(PhraseComponent))) {
}

PhraseComponent::PhraseComponent(EntityId id, const sol::table& table)
    : ComponentTemplate(id, std::type_index(typeid(PhraseComponent))) {

    meta::doForAllMembers<PhraseComponent>([this, &table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.get_name();
        sol::object value_obj = table[name];
        assert(value_obj.valid());
        auto value = value_obj.as<std::string>();
        member.set(*this, value);
    });

}

PhraseComponent::~PhraseComponent() {
}


json PhraseComponent::to_json() {
    auto j = json{};

    j["phrase"] = m_phrase;

    return j;
}

void PhraseComponent::load_json(const json & j) {
    m_phrase = j["phrase"].get<std::string>();
}

std::string PhraseComponent::get_phrase() const {
    return m_phrase;
}


void PhraseComponent::set_phrase(const std::string& phrase) {
    m_phrase = phrase;
}


int phraseEditCallback(ImGuiTextEditCallbackData *data) {
    auto o = reinterpret_cast<PhraseComponent*>(data->UserData);
    o->set_phrase(std::string(data->Buf));
    return 0;
}

void PhraseComponent::draw_component_inspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculate_showname().c_str(), &m_gui_open);


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

void PhraseComponent::expose_to_lua()
{

    LUA.new_usertype<PhraseComponent>(meta::get_name<PhraseComponent>(),


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "phrase", sol::property(&PhraseComponent::get_phrase, &PhraseComponent::set_phrase)

        );

}
