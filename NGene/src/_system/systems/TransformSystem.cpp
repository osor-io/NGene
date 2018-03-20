#include "TransformSystem.h"
#include <../_component/components/TransformComponent.h>
#include <../time/TimeManager.h>


TransformSystem::TransformSystem() {}


TransformSystem::~TransformSystem() {}

void TransformSystem::start_up() {

    /*
    We register the system so we can receive the entities with the components we are interested in.
    */
    registerSystem();
}

void TransformSystem::shut_down() {

}



void TransformSystem::update() {

    for (auto& e : m_entities) {
        auto transform = e.second->get_component<TransformComponent>();
        transform->m_previous_position = transform->m_position;
    }

}


void TransformSystem::register_entity(Entity& entity) {
    if (entity.has_component<TransformComponent>()) {
        m_entities[entity.get_id()] = &entity;
    }
}

void TransformSystem::deregister_entity(EntityId id) {
    m_entities.erase(id);
}


