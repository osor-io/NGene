#include "SimplePhraseComponent.h"
#include "../lua/LuaManager.h"


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

#define REGISTER_METHOD(method) #method , &SimplePhraseComponent::method


void SimplePhraseComponent::exposeToLua()
{

    LUA.new_usertype<SimplePhraseComponent>("SimplePhraseComponent",


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "phrase", sol::property(&SimplePhraseComponent::getPhrase, &SimplePhraseComponent::setPhrase)

        );

}


std::string SimplePhraseComponent::getPhrase() const {
    return m_phrase;
}


void SimplePhraseComponent::setPhrase(const std::string& phrase) {
    m_phrase = phrase;
}