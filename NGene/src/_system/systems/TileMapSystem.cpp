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
        /*
        @@TODO
        
        Do stuff with tile maps if we need to, we could check
        for a flag in the tile map component to see if it has been initialized
        (this flag hasn't been added yet).
        
        This way we can do more initialization
        stuff that calls to some manager or something else that doesn't make sense
        in the constructor of the component since those shouldn't have any logic in them.
        (Besides debug ones, construction ones, etc, etc).
        */
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


