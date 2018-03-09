#include "Entity.h"
#include "../lua/LuaManager.h"

Entity::Entity() {
}

Entity::~Entity() {
}

std::string Entity::getType() const {
    return m_type;
}

void Entity::setType(const std::string& type) {
    m_type = type;
}


void Entity::setType(std::string&& type) {
    m_type = std::move(type);
}

#include "../_component/components/SimpleGraphicsComponent.h"

#define GET_COMPONENT_NAME(type) #type
#define MAKE_STRING(s) #s
#define REGISTER_GET_COMPONENT(type) "get" #type , &Entity::getComponent< ## type ## >

void Entity::exposeToLua() {

    LUA.new_usertype<Entity>("Entity",

        // Constructors
        sol::constructors<Entity()>(),

        // Members
        REGISTER_GET_COMPONENT(SimpleGraphicsComponent)

        );

}
