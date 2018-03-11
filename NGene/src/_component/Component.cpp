#include "Component.h"
#include "ComponentManager.h"

Component::Component(std::type_index type) : m_type(type) {

    ComponentManager::get().addComponentInstance(m_type, this);

}

Component::~Component() {

    ComponentManager::get().removeComponentInstance(m_type, this);

};

