#include "SimpleGraphicsComponent.h"
#include <Debug.h>
#include "../lua/LuaManager.h"

SimpleGraphicsComponent::SimpleGraphicsComponent() : Component(std::type_index(typeid(SimpleGraphicsComponent))) {
}


SimpleGraphicsComponent::SimpleGraphicsComponent(const sol::table& table)
    : Component(std::type_index(typeid(SimpleGraphicsComponent))) {

    meta::doForAllMembers<SimpleGraphicsComponent>([this, &table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.getName();
        sol::object value_obj = table[name];
        assert(value_obj.valid());
        auto value = value_obj.as<std::string>();
        member.set(*this, value);
    });
}

SimpleGraphicsComponent::~SimpleGraphicsComponent() {
}

void SimpleGraphicsComponent::exposeToLua() {

    LUA.new_usertype<SimpleGraphicsComponent>("SimpleGraphicsComponent",


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "filename", sol::property(&SimpleGraphicsComponent::getFilename, &SimpleGraphicsComponent::setFilename)

        );

}


std::string SimpleGraphicsComponent::getFilename() const {
    return m_filename;
}


void SimpleGraphicsComponent::setFilename(const std::string& filename) {
    m_filename = filename;
}

sf::Sprite SimpleGraphicsComponent::getSprite() const {
    return m_sprite;
}

void SimpleGraphicsComponent::setSprite(const sf::Sprite & sprite) {
    m_sprite = sprite;
}

void SimpleGraphicsComponent::moveSprite(sf::Sprite && sprite) {
    m_sprite = std::move(sprite);
}
