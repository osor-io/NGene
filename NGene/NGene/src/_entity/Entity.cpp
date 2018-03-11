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


#include "../_component/components/SimpleGraphicsComponent.h"
#include "../_component/components/SimplePhraseComponent.h"

#define GET_COMPONENT_NAME(type) #type
#define MAKE_STRING(s) #s
#define REGISTER_GET_COMPONENT(type) "get" #type , &Entity::getComponent< ## type ## >

void Entity::exposeToLua() {

    /*
    Here we expose the members of the components
    */
    SimpleGraphicsComponent::exposeToLua();
    SimplePhraseComponent::exposeToLua();

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
        REGISTER_GET_COMPONENT(SimpleGraphicsComponent),
        REGISTER_GET_COMPONENT(SimplePhraseComponent)

        );

}
