#pragma once
#include "../System.h"
#include <map>
#include <memory>

class CameraSystem : public System<CameraSystem> {
    using EntitiesGroup = std::map<EntityId, Entity*>;
    friend class CRSP<CameraSystem>;
private:
    CameraSystem();
    ~CameraSystem();
public:

    void update() override;
    void start_up() override;
    void shut_down() override;
    void register_entity(Entity& entity) override;
    void deregister_entity(EntityId id) override;

private:
    EntitiesGroup m_entities;
};

