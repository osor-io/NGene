#include "SystemManager.h"



SystemManager::SystemManager(){

}


SystemManager::~SystemManager() {

}


void SystemManager::startUp() {

}


void SystemManager::shutDown() {
    m_systems.clear();
}


void SystemManager::registerSystemFunc(std::function<void(Entity& entity)> function){
	m_systems.push_back(function);
}


void SystemManager::registerEntityInSystems(Entity& entity){
	for(auto f : m_systems){
		f(entity);
	}
}
