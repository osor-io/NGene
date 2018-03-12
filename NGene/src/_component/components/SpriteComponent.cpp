#include "SpriteComponent.h"
#include <Debug.h>
#include "../lua/LuaManager.h"

SpriteComponent::SpriteComponent() : Component(std::type_index(typeid(SpriteComponent))) {
}


SpriteComponent::SpriteComponent(const sol::table& table)
    : Component(std::type_index(typeid(SpriteComponent))) {

    meta::doForAllMembers<SpriteComponent>([this, &table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.getName();
        sol::object value_obj = table[name];
        assert(value_obj.valid());
        auto value = value_obj.as<std::string>();
        member.set(*this, value);
    });
}

SpriteComponent::~SpriteComponent() {
}

void SpriteComponent::exposeToLua() {

    LUA.new_usertype<SpriteComponent>("SpriteComponent",


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "filename", sol::property(&SpriteComponent::getFilename, &SpriteComponent::setFilename)

        );

}


std::string SpriteComponent::getFilename() const {
    return m_filename;
}


void SpriteComponent::setFilename(const std::string& filename) {
    m_filename = filename;
}

sf::Sprite SpriteComponent::getSprite() const {
    return m_sprite;
}

void SpriteComponent::setSprite(const sf::Sprite & sprite) {
    m_sprite = sprite;
}

void SpriteComponent::moveSprite(sf::Sprite && sprite) {
    m_sprite = std::move(sprite);
}
