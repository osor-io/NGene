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

    auto target = RenderManager::get().get_main_render_target();

    for (auto& v : m_sorted_entities) {
        std::sort(v.second.begin(), v.second.end(), [](Entity * a, Entity * b) ->bool {
            return a->get_component<TransformComponent>()->m_position.y < b->get_component<TransformComponent>()->m_position.y;
        });
    }

    /*
    We iterate through all the entities we are interested in.
    */
    for (auto& v : m_sorted_entities) {

        for (auto& entity : v.second) {
            if (entity->is_enabled()) {
                auto graphics_component = entity->get_component<SpriteComponent>();
                auto transform_component = entity->get_component<TransformComponent>();

                auto sprite = graphics_component->get_sprite_ptr();
                sprite->setPosition(transform_component->get_position());
                target->draw(*sprite);
            }
        }

    }

}


void RenderSystem::register_entity(Entity& entity) {
    if (entity.has_component<SpriteComponent>() && entity.has_component<TransformComponent>()) {
        m_entities[entity.get_id()] = &entity;
        auto spriteComponent = entity.get_component<SpriteComponent>();
        m_sorted_entities[spriteComponent->m_layer].push_back(&entity);
    }
}

void RenderSystem::deregister_entity(EntityId id) {
    m_entities.erase(id);
    auto e = EntityManager::get().get_entity(id);
    auto& v = m_sorted_entities[e->get_component<SpriteComponent>()->m_layer];
    v.erase(std::remove(v.begin(), v.end(), e), v.end());
}


