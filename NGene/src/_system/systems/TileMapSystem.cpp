#include "TileMapSystem.h"
#include <../_component/components/TiledMapComponent.h>
#include <../time/TimeManager.h>


TileMapSystem::TileMapSystem() {}


TileMapSystem::~TileMapSystem() {}

void TileMapSystem::start_up() {

    /*
    We register the system so we can receive the entities with the components we are interested in.
    */
    registerSystem();
}

void TileMapSystem::shut_down() {

}



void TileMapSystem::update() {

    for (auto& e : m_entities) {
        //@@TODO: Do stuff with tile maps if we need to
    }

}


void TileMapSystem::register_entity(Entity& entity) {
    if (entity.has_component<TiledMapComponent>()) {
        m_entities[entity.get_id()] = &entity;
    }
}

void TileMapSystem::deregister_entity(EntityId id) {
    m_entities.erase(id);
}


