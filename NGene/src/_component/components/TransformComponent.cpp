#include "TransformComponent.h"
#include "../lua/LuaManager.h"
#include <Debug.h>

TransformComponent::TransformComponent() : Component(std::type_index(typeid(TransformComponent))) {}


TransformComponent::TransformComponent(const sol::table& table) : Component(std::type_index(typeid(TransformComponent))) {

 
    sol::object value_x = table["x"];
    sol::object value_y = table["y"];
    assert(value_x.valid() && value_y.valid());

    m_position = sf::Vector2f(value_x.as<float>(), value_y.as<float>());
    
}


TransformComponent::~TransformComponent() {}


sf::Vector2f TransformComponent::getPosition() const {
    return m_position;
}


void TransformComponent::setPosition(const sf::Vector2f & position) {
    m_position = position;
}


LuaVector2f TransformComponent::luaGetPosition() const {
    return m_position;
}


void TransformComponent::luaSetPosition(const LuaVector2f & position) {
    m_position = LuaVector2f::toSf(position);
}

void TransformComponent::exposeToLua() {

    LUA.new_usertype<LuaVector2f>("Vector2f",


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "x", &LuaVector2f::x,
        "y", &LuaVector2f::y

        );

    LUA.new_usertype<TransformComponent>("TransformComponent",


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "position", sol::property(&TransformComponent::luaGetPosition, &TransformComponent::luaSetPosition)

        );
}



