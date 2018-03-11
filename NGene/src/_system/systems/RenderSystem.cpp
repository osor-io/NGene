#include "RenderSystem.h"
#include "../SystemManager.h"
#include "Debug.h"
#include "../_component/components/SimpleGraphicsComponent.h"
#include "../_component/components/TransformComponent.h"

RenderSystem::RenderSystem()
{
}


RenderSystem::~RenderSystem()
{
}

void RenderSystem::update() {
    for (auto& e : m_entities) {
        auto graphicsComponent = e.second->getComponent<SimpleGraphicsComponent>();
        auto transformComponent = e.second->getComponent<TransformComponent>();

        LOG("ENTITY [" << e.second->getId() << "]: " << "We would draw the sprite from " <<
            graphicsComponent->getFilename() <<
            " in the position " << transformComponent->getPosition().x <<
            ", " << transformComponent->getPosition().y);
    }
}

void RenderSystem::startUp() {
    SystemManager::get().registerSystemFunc([this](Entity& entity) {
        registerEntity(entity);
    });
}


void RenderSystem::shutDown() {
}


void RenderSystem::registerEntity(Entity& entity) {
    if (entity.hasComponent<SimpleGraphicsComponent>() && entity.hasComponent<TransformComponent>()) {
        m_entities[entity.getId()] = &entity;
    }
}

