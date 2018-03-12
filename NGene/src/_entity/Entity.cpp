#include "Entity.h"
#include "../lua/LuaManager.h"

Entity::Entity(EntityId id) : m_id(id) {
}

Entity::~Entity() {
}

EntityId Entity::getId() const
{
    return m_id;
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

void Entity::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool Entity::isEnabled() {
    return m_enabled;
}

#include "../_component/components/TransformComponent.h"
#include "../_component/components/SpriteComponent.h"
#include "../_component/components/PhraseComponent.h"

#define GET_COMPONENT_NAME(type) #type
#define MAKE_STRING(s) #s
#define REGISTER_GET_COMPONENT(type) "get" #type , &Entity::getComponent< ## type ## >

void Entity::exposeToLua() {

    /*
    Here we expose the members of the components
    */
    TransformComponent::exposeToLua();
    SpriteComponent::exposeToLua();
    PhraseComponent::exposeToLua();

    /*
    And here a way to access them from the entity that contains them
    */
    LUA.new_usertype<Entity>("Entity",

        /*
        We don't allow constructors to be called from Lua since
        we want the EntityManager to act as a factory for all the entities
        */

        // Members
        /*
        Add here all the components that we want to expose to Lua via "getComponentName()" functions
        */
        REGISTER_GET_COMPONENT(TransformComponent),
        REGISTER_GET_COMPONENT(SpriteComponent),
        REGISTER_GET_COMPONENT(PhraseComponent),

        "id", sol::readonly_property(&Entity::getId),
        "type", sol::readonly_property(&Entity::getType),
        "enabled", sol::property(&Entity::isEnabled, &Entity::setEnabled)

        );

}
