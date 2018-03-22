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
        draw_layer(target, l);
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
        draw_layer(target, l);
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
        assert(m_current_map == nullptr);
        assert(m_background_layers.size() == 0);
        assert(m_foreground_layers.size() == 0);

        m_current_map_entity = &entity;
        auto tiledmap_component = m_current_map_entity->get_component<TiledMapComponent>();
        if (tiledmap_component->m_map_ready) {

            m_current_map = &tiledmap_component->m_map;

            assert(m_current_map != nullptr);
            assert(m_current_map->layers.size() > 0);
            assert(tiledmap_component->m_main_layer >= 0);
            assert(m_current_map->layers.size() > tiledmap_component->m_main_layer);


            for (auto i = 0; i <= tiledmap_component->m_main_layer; ++i) {
                m_background_layers.push_back(&m_current_map->layers[i]);
            }

            for (auto i = tiledmap_component->m_main_layer + 1; i < m_current_map->layers.size(); ++i) {
                m_foreground_layers.push_back(&m_current_map->layers[i]);
            }
        }
    }
}

void RenderSystem::deregister_entity(EntityId id) {

    if (m_current_map_entity && id == m_current_map_entity->get_id()) {
        m_current_map_entity = nullptr;
        m_current_map = nullptr;
        m_background_layers.clear();
        m_foreground_layers.clear();
    }
    else if (m_entities.find(id) != m_entities.end()) {
        m_entities.erase(id);
        auto e = EntityManager::get().get_entity(id);
        auto& v = m_sorted_entities[e->get_component<SpriteComponent>()->m_layer];
        v.erase(std::remove(v.begin(), v.end(), e), v.end());

    }
}

void RenderSystem::draw_layer(sf::RenderTarget* target, Layer * layer) {

    auto delta_time = DELTA_MILLISECONDS;

    auto i = -1;

    for (auto& tr : layer->tile_references) {
        ++i;
        if (!tr.tile) continue;

        auto sprite_to_render = sf::Sprite{};

        if (tr.tile->animated) {
            
            auto number_of_frames = tr.tile->animated_sprites->size();
            tr.elapsed_in_frame += delta_time;
            if (tr.elapsed_in_frame > tr.current_duration) {
                tr.current_frame = (tr.current_frame + 1) % number_of_frames;
                tr.elapsed_in_frame -= tr.current_duration;
                auto& next_sprite_tuple = (*tr.tile->animated_sprites.get())[tr.current_frame];
                tr.current_duration = std::get<1>(next_sprite_tuple);
                sprite_to_render = std::get<0>(next_sprite_tuple);
            }
            else {
                sprite_to_render = std::get<0>((*tr.tile->animated_sprites.get())[tr.current_frame]);
            }

        }
        else {
            sprite_to_render = tr.tile->sprite;
        }

        sprite_to_render.setPosition(m_current_map->tile_coordinates[i]);

        target->draw(sprite_to_render);
    }
}


