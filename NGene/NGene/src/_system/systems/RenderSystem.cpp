#include "RenderSystem.h"
#include "../SystemManager.h"
#include "Debug.h"
#include "../_component/components/SimpleGraphicsComponent.h"

RenderSystem::RenderSystem()
{
}


RenderSystem::~RenderSystem()
{
}

void RenderSystem::update() {

}

void RenderSystem::startUp() {
    SystemManager::get().registerSystemFunc([this](Entity& entity) {
        registerEntity(entity);
    });
}


void RenderSystem::shutDown() {
}


void RenderSystem::registerEntity(Entity& entity){

    if (entity.hasComponent<SimpleGraphicsComponent>()) {
    }
    else {
    }

    /*
    @@DOING: Register the entity if we are interested in it
    @@TODO: Register the entity if we are interested in it
    */
}

