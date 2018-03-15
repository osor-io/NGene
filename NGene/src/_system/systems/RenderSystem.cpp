#include "RenderSystem.h"
#include "../SystemManager.h"
#include "../render/RenderManager.h"
#include "Debug.h"
#include "../_component/components/SpriteComponent.h"
#include "../_component/components/TransformComponent.h"
#include "../time/TimeManager.h"
#include "../_entity/EntityManager.h"

RenderSystem::RenderSystem() {}


RenderSystem::~RenderSystem() {}

void RenderSystem::start_up() {

    /*
    We register the system so we can receive the entities with the components we are interested in.
    */
    registerSystem();
}

void RenderSystem::shut_down() {

}



void RenderSystem::update() {

    auto target = RenderManager::get().getMainRenderTarget();

    for (auto& v : m_sortedEntities) {
        std::sort(v.second.begin(), v.second.end(), [](Entity * a, Entity * b) ->bool {
            return a->getComponent<TransformComponent>()->m_position.y < b->getComponent<TransformComponent>()->m_position.y;
        });
    }

    /*
    We iterate through all the entities we are interested in.
    */
    for (auto& v : m_sortedEntities) {

        for (auto& entity : v.second) {
            if (entity->isEnabled()) {
                auto graphicsComponent = entity->getComponent<SpriteComponent>();
                auto transformComponent = entity->getComponent<TransformComponent>();

                auto sprite = graphicsComponent->get_sprite_ptr();
                sprite->setPosition(transformComponent->get_position());
                target->draw(*sprite);
            }
        }

    }

}


void RenderSystem::registerEntity(Entity& entity) {
    if (entity.hasComponent<SpriteComponent>() && entity.hasComponent<TransformComponent>()) {
        m_entities[entity.getId()] = &entity;
        auto spriteComponent = entity.getComponent<SpriteComponent>();
        m_sortedEntities[spriteComponent->m_layer].push_back(&entity);
    }
}

void RenderSystem::deregisterEntity(EntityId id) {
    m_entities.erase(id);
    auto e = EntityManager::get().getEntity(id);
    auto& vec = m_sortedEntities[e->getComponent<SpriteComponent>()->m_layer];
    vec.erase(std::remove(vec.begin(), vec.end(), e), vec.end());
}


