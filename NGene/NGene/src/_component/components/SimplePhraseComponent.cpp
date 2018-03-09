#include "SimplePhraseComponent.h"

#include <iostream>


SimplePhraseComponent::SimplePhraseComponent() : Component(std::type_index(typeid(SimplePhraseComponent))) {
}

SimplePhraseComponent::SimplePhraseComponent(const sol::table& table)
    : Component(std::type_index(typeid(SimplePhraseComponent))) {

    meta::doForAllMembers<SimplePhraseComponent>([this,&table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.getName();
        sol::object value_obj = table[name];
        auto value = value_obj.as<std::string>();
        member.set(*this, value);
    });

}

SimplePhraseComponent::~SimplePhraseComponent() {
}


std::string SimplePhraseComponent::getPhrase() const {
    return m_phrase;
}


void SimplePhraseComponent::setPhrase(const std::string& phrase) {
    m_phrase = phrase;
}


void SimplePhraseComponent::setPhrase(std::string&& phrase) {
    m_phrase = std::move(phrase);
}