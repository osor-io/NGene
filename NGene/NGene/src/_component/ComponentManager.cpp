#include "ComponentManager.h"
#include <Debug.h>

ComponentManager::ComponentManager() {

}


ComponentManager::~ComponentManager() {

}

void ComponentManager::addComponentInstance(const std::type_index& type, Component* component) {
    m_instanceMap[type].push_back(component);
}

void ComponentManager::removeComponentInstance(const std::type_index& type, Component* component) {
    auto& vector = m_instanceMap[type];
    vector.erase(std::remove(vector.begin(), vector.end(), component), vector.end());
}