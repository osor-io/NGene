#include "CollisionSystem.h"
#include <../_component/components/CollisionComponent.h>
#include <../time/TimeManager.h>
#include <../physics/ChunkManager.h>


CollisionSystem::CollisionSystem() {}


CollisionSystem::~CollisionSystem() {}

void CollisionSystem::start_up() {

    /*
    We register the system so we can receive the entities with the components we are interested in.
    */
    registerSystem();
}

void CollisionSystem::shut_down() {

}



void CollisionSystem::update() {

    const auto& grouped_entities = ChunkManager::get().get_grouped_entities_with_components<TransformComponent, CollisionComponent>();


    for (const auto & v : grouped_entities) {
        if (v.second.size() > 1) {
            LOG("Two entities might be colliding in chunk (" << v.first.first << ", " << v.first.second << ")");
        }
    }


}


void CollisionSystem::register_entity(Entity& entity) {}

void CollisionSystem::deregister_entity(EntityId id) {}


