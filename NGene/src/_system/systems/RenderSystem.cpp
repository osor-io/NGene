#include "RenderSystem.h"
#include "../SystemManager.h"
#include "Debug.h"
#include "../_component/components/SpriteComponent.h"
#include "../_component/components/TransformComponent.h"

RenderSystem::RenderSystem()
{
}


RenderSystem::~RenderSystem()
{
}

void RenderSystem::update() {
    for (auto& e : m_entities) {
        auto graphicsComponent = e.second->getComponent<SpriteComponent>();
        auto transformComponent = e.second->getComponent<TransformComponent>();

        LOG("ENTITY [" << e.second->getId() << "]: " << "We would draw the sprite from " <<
            graphicsComponent->getFilename() <<
            " in the position " << transformComponent->getPosition().x <<
            ", " << transformComponent->getPosition().y);
    }
}

void RenderSystem::startUp() {
    registerSystem();
}

void RenderSystem::registerSystem() {
    SystemManager::get().addSystemRegisterFunc([this](Entity& entity) {
        registerEntity(entity);
    });
    SystemManager::get().addSystemDeregisterFunc([this](EntityId id) {
        deregisterEntity(id);
    });
}

void RenderSystem::shutDown() { }

void RenderSystem::registerEntity(Entity& entity) {
    if (entity.hasComponent<SpriteComponent>() && entity.hasComponent<TransformComponent>()) {
        m_entities[entity.getId()] = &entity;
    }
}

void RenderSystem::deregisterEntity(EntityId id) {
    m_entities.erase(id);
}


