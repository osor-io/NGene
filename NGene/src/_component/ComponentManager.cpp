#include "ComponentManager.h"
#include <Debug.h>
#include "../debug/LoggingManager.h"

ComponentManager::ComponentManager() {

}


ComponentManager::~ComponentManager() {

}

void ComponentManager::start_up() {

}

void ComponentManager::shut_down() {

#ifdef _DEBUG
    for (auto& v : m_instance_map) {
        assert(v.second.size() == 0);
    }
#endif

    m_instance_map.clear();
}

void ComponentManager::add_component_instance(const std::type_index& type, Component* component) {
    m_instance_map[type].push_back(component);
}

void ComponentManager::remove_component_instance(const std::type_index& type, Component* component) {
    auto& v = m_instance_map[type];
    v.erase(std::remove(v.begin(), v.end(), component), v.end());
}

bool ComponentManager::supports_component(const std::string & name) const {
    auto it = m_type_map.find(name);
    return (it != m_type_map.end() && (m_factory_map.find(it->second) != m_factory_map.end()));
}
