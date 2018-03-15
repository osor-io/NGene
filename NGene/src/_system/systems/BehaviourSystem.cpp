#include "BehaviourSystem.h"
#include <../_component/components/BehaviourComponent.h>
#include <../time/TimeManager.h>


BehaviourSystem::BehaviourSystem() {}


BehaviourSystem::~BehaviourSystem() {}

void BehaviourSystem::start_up() {

    /*
    We register the system so we can receive the entities with the components we are interested in.
    */
    registerSystem();
}

void BehaviourSystem::shut_down() {

}



void BehaviourSystem::update() {

    for (auto& e : m_entities) {
        e.second->getComponent<BehaviourComponent>()->get_on_update()(*(e.second), DELTA_SECONDS);
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


