#include "BehaviourSystem.h"
#include <../_component/components/BehaviourComponent.h>
#include <../time/TimeManager.h>


BehaviourSystem::BehaviourSystem() {}


BehaviourSystem::~BehaviourSystem() {}

void BehaviourSystem::startUp() {

    /*
    We register the system so we can receive the entities with the components we are interested in.
    */
    registerSystem();
}

void BehaviourSystem::shutDown() {

}



void BehaviourSystem::update() {

    for (auto& e : m_entities) {
        e.second->getComponent<BehaviourComponent>()->getOnUpdate()(*(e.second), DELTA_SECONDS);
    }

}


void BehaviourSystem::registerEntity(Entity& entity) {
    if (entity.hasComponent<BehaviourComponent>()) {
        m_entities[entity.getId()] = &entity;
    }
}

void BehaviourSystem::deregisterEntity(EntityId id) {
    m_entities.erase(id);
}


