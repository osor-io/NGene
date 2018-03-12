#include "RenderSystem.h"
#include "../SystemManager.h"
#include "Debug.h"
#include "../_component/components/SpriteComponent.h"
#include "../_component/components/TransformComponent.h"
#include "../time/TimeManager.h"

RenderSystem::RenderSystem() {}


RenderSystem::~RenderSystem() {}

void RenderSystem::startUp() {

    /*
    We register the system so we can receive the entities with the components we are interested in.
    */
    registerSystem();
}

void RenderSystem::shutDown() {

}



void RenderSystem::update() {
    /*
    We iterate through all the entities we are interested in.
    */
    for (auto& e : m_entities) {

        auto& entity = e.second;
        if (entity->isEnabled()) {
            auto graphicsComponent = entity->getComponent<SpriteComponent>();
            auto transformComponent = entity->getComponent<TransformComponent>();

            /*
            LOG("ENTITY [" << e.second->getId() << "]: " << "We would draw the sprite from " <<
                graphicsComponent->getFilename() <<
                " in the position " << transformComponent->getPosition().x <<
                ", " << transformComponent->getPosition().y);
            */
        }
    }
}


void RenderSystem::registerSystem() {
    SystemManager::get().addSystemRegisterFunc([this](Entity& entity) {
        registerEntity(entity);
    });
    SystemManager::get().addSystemDeregisterFunc([this](EntityId id) {
        deregisterEntity(id);
    });
}

void RenderSystem::registerEntity(Entity& entity) {
    if (entity.hasComponent<SpriteComponent>() && entity.hasComponent<TransformComponent>()) {
        m_entities[entity.getId()] = &entity;
    }
}

void RenderSystem::deregisterEntity(EntityId id) {
    m_entities.erase(id);
}


