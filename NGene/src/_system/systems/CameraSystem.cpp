#include "CameraSystem.h"
#include <../_component/components/CameraComponent.h>
#include <../_component/components/TransformComponent.h>
#include <../time/TimeManager.h>
#include "../debug/LoggingManager.h"
#include "../render/RenderManager.h"


CameraSystem::CameraSystem() {}


CameraSystem::~CameraSystem() {}

void CameraSystem::start_up() {

    /*
    We register the system so we can receive the entities with the components we are interested in.
    */
    registerSystem();
}

void CameraSystem::shut_down() {

}



void CameraSystem::update() {

    Entity * main_camera_entity = nullptr;

    for (auto& e : m_entities) {
        const auto& camera = e.second->get_component<CameraComponent>();
        if (camera->m_main_camera) {
            if (main_camera_entity) {
                LOGF("There is more than one main camera!");
            }
            main_camera_entity = e.second;
        }
    }


    auto render_target = RenderManager::get().get_main_render_target();

    if (main_camera_entity) {
        auto center = main_camera_entity->get_component<TransformComponent>()->m_position;
        auto view = render_target->getView();
        view.setCenter(center);
        render_target->setView(view);
    }
    else {
        render_target->setView(render_target->getDefaultView());
    }

}


void CameraSystem::register_entity(Entity& entity) {
    if (entity.has_component<CameraComponent>() && entity.has_component<TransformComponent>()) {
        m_entities[entity.get_id()] = &entity;
    }
}

void CameraSystem::deregister_entity(EntityId id) {
    m_entities.erase(id);
}


