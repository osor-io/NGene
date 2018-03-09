#include "SimpleGraphicsComponent.h"
#include <Debug.h>

SimpleGraphicsComponent::SimpleGraphicsComponent() : Component(std::type_index(typeid(SimpleGraphicsComponent))) {
}


SimpleGraphicsComponent::SimpleGraphicsComponent(const sol::table& table)
    : Component(std::type_index(typeid(SimpleGraphicsComponent))) {
    meta::doForAllMembers<SimpleGraphicsComponent>([this, &table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.getName();
        sol::object value_obj = table[name];
        auto value = value_obj.as<std::string>();
        member.set(*this, value);
    });
}

SimpleGraphicsComponent::~SimpleGraphicsComponent() {
}


std::string SimpleGraphicsComponent::getFilename() const {
    return m_filename;
}


void SimpleGraphicsComponent::setFilename(const std::string& filename) {
    m_filename = filename;
}


void SimpleGraphicsComponent::setFilename(std::string&& filename) {
    m_filename = std::move(filename);
}