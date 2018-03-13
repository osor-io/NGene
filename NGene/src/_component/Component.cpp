#include "Component.h"
#include "ComponentManager.h"

Component::Component(EntityId id, std::type_index type) : m_type(type), m_parentId(id) {
    ComponentManager::get().addComponentInstance(m_type, this);
}

Component::~Component() {
    ComponentManager::get().removeComponentInstance(m_type, this);
};

