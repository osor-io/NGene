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

    TiledMapComponent* tilemap = nullptr;


    for (auto& v : m_sorted_entities) {
        std::sort(v.second.begin(), v.second.end(), [](Entity * a, Entity * b) ->bool {
            return a->get_component<TransformComponent>()->m_position.y < b->get_component<TransformComponent>()->m_position.y;
        });
    }


    /*
    First we render the layers up to the main one
    */
    for (const auto& l : m_background_layers) {
        target->draw(*l);
    }


    /*
    We iterate through all the entities we are interested in.
    */
    for (auto& v : m_sorted_entities) {

        for (auto& entity : v.second) {
            if (entity->is_enabled() && entity->is_in_relevant_chunk()) {
                auto graphics_component = entity->get_component<SpriteComponent>();
                auto transform_component = entity->get_component<TransformComponent>();

                auto sprite = graphics_component->get_sprite_ptr();
                sprite->setPosition(transform_component->get_position());
                target->draw(*sprite);
            }
        }

    }


    /*
    Now we render the foreground layers
    */
    for (const auto& l : m_foreground_layers) {
        target->draw(*l);
    }

}


void RenderSystem::register_entity(Entity& entity) {
    if (entity.has_component<SpriteComponent>() && entity.has_component<TransformComponent>()) {
        m_entities[entity.get_id()] = &entity;
        auto spriteComponent = entity.get_component<SpriteComponent>();
        m_sorted_entities[spriteComponent->m_layer].push_back(&entity);
    }
    else if (entity.has_component<TiledMapComponent>()) {

        /*
        We are trying to load a map when we already have one :(
        */
        assert(m_current_map_entity == nullptr);
        assert(m_background_layers.size() == 0);
        assert(m_foreground_layers.size() == 0);


        m_current_map_entity = &entity;
        auto tiledmap = m_current_map_entity->get_component<TiledMapComponent>();
        if (tiledmap->m_map_ready) {

            const auto& layers = tiledmap->m_map.getLayers();

            for (auto i = 0; i <= tiledmap->m_main_layer; ++i) {
                if (layers[i].get() && layers[i].get()->getType() == tmx::Layer::Type::Tile) {
                    m_background_layers.push_back(std::make_unique<MapLayer>(tiledmap->m_map, i));
                }
            }

            for (auto i = tiledmap->m_main_layer + 1; i < layers.size(); ++i) {
                if (layers[i].get() && layers[i].get()->getType() == tmx::Layer::Type::Tile) {
                    m_foreground_layers.push_back(std::make_unique<MapLayer>(tiledmap->m_map, i));
                }
            }

        }

    }
}

void RenderSystem::deregister_entity(EntityId id) {

    if (m_current_map_entity && id == m_current_map_entity->get_id()) {
        m_current_map_entity = nullptr;
        m_background_layers.clear();
        m_foreground_layers.clear();
    }
    else {

        m_entities.erase(id);
        auto e = EntityManager::get().get_entity(id);
        auto& v = m_sorted_entities[e->get_component<SpriteComponent>()->m_layer];
        v.erase(std::remove(v.begin(), v.end(), e), v.end());
    }
}


