#include "ComponentManager.h"
#include <Debug.h>

ComponentManager::ComponentManager() {

}


ComponentManager::~ComponentManager() {

}

void ComponentManager::start_up() {

}

void ComponentManager::shut_down() {
    m_instance_map.clear();
}

void ComponentManager::add_component_instance(const std::type_index& type, Component* component) {
    m_instance_map[type].push_back(component);
}

void ComponentManager::remove_component_instance(const std::type_index& type, Component* component) {
    auto& v = m_instance_map[type];
    v.erase(std::remove(v.begin(), v.end(), component), v.end());
}