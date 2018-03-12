#pragma once
#include "../System.h"
#include <map>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <memory>

/**
System that handles all the rendering functionality in the application

@@TODO: Implement ImGUI rendering elements such as console, entity viewer, etc.
@@TODO: Implement access and rendering to all "renderable" components

*/
class RenderSystem : public System<RenderSystem> {
    using EntitiesGroup = std::map<EntityId, Entity*>;

    friend class CRSP<RenderSystem>;
private:
    RenderSystem();
    ~RenderSystem();
public:

    void update() override;
    void startUp() override;
    void shutDown() override;
    void registerEntity(Entity& entity) override;
    void deregisterEntity(EntityId id) override;
    void registerSystem() override;

private:
    EntitiesGroup m_entities;

};

