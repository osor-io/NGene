#include "Component.h"
#include "ComponentManager.h"

Component::Component(EntityId id, std::type_index type) : m_type(type), m_parent_id(id) {
    ComponentManager::get().add_component_instance(m_type, this);
}

Component::~Component() {
    ComponentManager::get().remove_component_instance(m_type, this);
};

