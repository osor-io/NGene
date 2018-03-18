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
        //@@TODO: Check if entity is active and is in relevant chunk
        e.second->get_component<BehaviourComponent>()->get_on_update()(*(e.second), DELTA_SECONDS);
    }

}


void BehaviourSystem::register_entity(Entity& entity) {
    if (entity.has_component<BehaviourComponent>()) {
        m_entities[entity.get_id()] = &entity;
    }
}

void BehaviourSystem::deregister_entity(EntityId id) {
    m_entities.erase(id);
}


