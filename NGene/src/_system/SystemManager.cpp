#include "SystemManager.h"



SystemManager::SystemManager() {

}


SystemManager::~SystemManager() {

}


void SystemManager::start_up() {

}


void SystemManager::shut_down() {
    m_registerFuncs.clear();
    m_deregisterFuncs.clear();
}


void SystemManager::addSystemRegisterFunc(std::function<void(Entity& entity)> function) {
    m_registerFuncs.push_back(function);
}

void SystemManager::addSystemDeregisterFunc(std::function<void(EntityId entity)> function) {
    m_deregisterFuncs.push_back(function);
}


void SystemManager::registerEntityInSystems(Entity& entity) {
    for (auto f : m_registerFuncs) {
        f(entity);
    }
}

void SystemManager::deregisterEntityInSystems(EntityId id) {
    for (auto f : m_deregisterFuncs) {
        f(id);
    }
}
