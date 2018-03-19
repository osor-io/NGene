#pragma once
#include "../System.h"
#include <map>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <../_component/components/TransformComponent.h>
#include "../_component/components/TiledMapComponent.h"


class RenderSystem : public System<RenderSystem> {
    using EntitiesGroup = std::map<EntityId, Entity*>;
    using LayerVectors = std::map<unsigned int, std::vector<Entity*>>;


    friend class CRSP<RenderSystem>;
private:
    RenderSystem();
    ~RenderSystem();
public:

    void update() override;
    void start_up() override;
    void shut_down() override;
    void register_entity(Entity& entity) override;
    void deregister_entity(EntityId id) override;

private:
    Entity * m_current_map_entity{ nullptr };
    EntitiesGroup m_entities;
    LayerVectors m_sorted_entities;

    std::vector<std::unique_ptr<MapLayer>> m_background_layers{};
    std::vector<std::unique_ptr<MapLayer>> m_foreground_layers{};

};

