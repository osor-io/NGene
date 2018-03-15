#include "SystemManager.h"



SystemManager::SystemManager() {

}


SystemManager::~SystemManager() {

}


void SystemManager::start_up() {

}


void SystemManager::shut_down() {
    m_register_funcs.clear();
    m_deregister_funcs.clear();
}


void SystemManager::add_system_register_func(std::function<void(Entity& entity)> function) {
    m_register_funcs.push_back(function);
}

void SystemManager::add_system_deregister_func(std::function<void(EntityId entity)> function) {
    m_deregister_funcs.push_back(function);
}


void SystemManager::register_entity_in_systems(Entity& entity) {
    for (auto f : m_register_funcs) {
        f(entity);
    }
}

void SystemManager::deregister_entity_in_systems(EntityId id) {
    for (auto f : m_deregister_funcs) {
        f(id);
    }
}
