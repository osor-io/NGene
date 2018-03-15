#pragma once
#include "../System.h"
#include <map>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <../_component/components/TransformComponent.h>


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
    void registerEntity(Entity& entity) override;
    void deregisterEntity(EntityId id) override;

private:
    EntitiesGroup m_entities;
    LayerVectors m_sortedEntities;
};

