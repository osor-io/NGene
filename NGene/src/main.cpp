#include "./utils/Debug.h"
#include "./File.h"

#include "./physics/ChunkManager.h" //@@TODO: Check that its start_up/shut_down is well positioned compared to other managers
#include "./debug/LoggingManager.h"
#include "./resources/TextFileManager.h"
#include "./resources/TextureManager.h"
#include "./input/InputManager.h"
#include "./time/TimeManager.h"
#include "./window/WindowManager.h"
#include "./render/RenderManager.h"
#include "./debug/DeveloperModeManager.h"
#include "./lua/LuaManager.h"

#include "./_entity/EntityManager.h"
#include "./_component/ComponentManager.h"
#include "./_system/SystemManager.h"

#include "./_system/systems/TileMapSystem.h"
#include "./_system/systems/InputSystem.h"
#include "./_system/systems/BehaviourSystem.h"
#include "./_system/systems/CameraSystem.h"
#include "./_system/systems/CollisionSystem.h"
#include "./_system/systems/TransformSystem.h"
#include "./_system/systems/RenderSystem.h"


void start_up() {

    ChunkManager::get().start_up();
    LoggingManager::get().start_up();
    TextFileManager::get().start_up();
    TextureManager::get().start_up();
    LuaManager::get().start_up();
    TimeManager::get().start_up();
    WindowManager::get().start_up();
    RenderManager::get().start_up();
    DeveloperModeManager::get().start_up();
    InputManager::get().start_up();

    SystemManager::get().start_up();
    ComponentManager::get().start_up();
    EntityManager::get().start_up();

    TileMapSystem::get().start_up();
    InputSystem::get().start_up();
    BehaviourSystem::get().start_up();
    CameraSystem::get().start_up();
    CollisionSystem::get().start_up();
    TransformSystem::get().start_up();
    RenderSystem::get().start_up();

}

void shut_down() {

    RenderSystem::get().shut_down();
    TransformSystem::get().shut_down();
    CollisionSystem::get().shut_down();
    CameraSystem::get().shut_down();
    BehaviourSystem::get().shut_down();
    InputSystem::get().shut_down();
    TileMapSystem::get().shut_down();

    EntityManager::get().shut_down();
    ComponentManager::get().shut_down();
    SystemManager::get().shut_down();

    InputManager::get().shut_down();
    DeveloperModeManager::get().shut_down();
    RenderManager::get().shut_down();
    WindowManager::get().shut_down();
    TimeManager::get().shut_down();
    LuaManager::get().shut_down();
    TextureManager::get().shut_down();
    TextFileManager::get().shut_down();
    LoggingManager::get().shut_down();
    ChunkManager::get().shut_down();
}

void load_default_state() {

    /*
    auto s = TextFileManager::get().get_scoped_resource("res/states/default_state.json");
    auto j = json::parse(*s.resource);
    EntityManager::get().clear_and_load_entities(j);
    */

    //If we want to check performance with more than one entity
    for (int i = 0; i < 3; ++i) {
    const auto id = EntityManager::get().request_load_entity("DynamicObject");
    EntityManager::get().update_entities();
    auto entity = EntityManager::get().get_entity(id);
    entity->set_name("All Mighty Entity");
    entity->get_component<TransformComponent>()->set_position(sf::Vector2f(300.f, 300.f));
    }
    
    const auto static_id_1 = EntityManager::get().request_load_entity("StaticObject");
    const auto static_id_2 = EntityManager::get().request_load_entity("StaticObject");
    const auto map_id = EntityManager::get().request_load_entity("DefaultMap");
    const auto camera_id = EntityManager::get().request_load_entity("DefaultCamera");

}

void access_entities_from_lua() {
    LUA.script(R"(
            print("Do we have an entity with the id 0? ", hasEntity(0))
            print("Getting Entity 0")
            local entity = getEntity(0);
            print("Entity returned: ", entity)
            if(entity) then
                print("The entity ", entity.type , " with the id " , entity.id , " existed, lets see if it has a phrase component")
                entity.enabled = false
                print("Is our entity enabled? ", entity.enabled)
                entity.enabled = true
                print("Is it now? ", entity.enabled)
                local phrase = entity:getPhraseComponent()
                if(phrase) then
                    print("It does! So let's hear what it has to say")
                    print("Our phrase is: ", phrase.phrase)
                else
                    print("It doesn't, so it'll just shut up")
                end
                local transform = entity:getTransformComponent()
                if(transform) then
                    print("Oh, it also has a position!")
                    local position = transform.position
                    print("Position x: ", position.x)
                    print("Position y: ", position.y)
                else
                    print("No Transform Available")
                end
            end
            print("\n")
        )");
}

#include "./_component/components/TransformComponent.h"
#include "./_component/components/SpriteComponent.h"

inline void tick() {

    ChunkManager::get().update_entity_chunks();

    TileMapSystem::get().update();
    InputSystem::get().update();
    BehaviourSystem::get().update();
    CameraSystem::get().update();
    CollisionSystem::get().update();

    TransformSystem::get().update();
}

inline void render() {

    WindowManager::get().fill_events();
    RenderManager::get().begin_frame();
    /*
    Everything that renders something goes in the
    next scope
    */
    {
        RenderSystem::get().update();
    }
    RenderManager::get().end_frame();

}

inline void post_render() {
    EntityManager::get().update_entities();
}


int main() {
    start_up();
    load_default_state();


    while (WindowManager::get().is_window_open()) {

        auto beg = TimeManager::get().query_cycle_counter();

        tick();

        auto end = TimeManager::get().query_cycle_counter();
        auto ms = TimeManager::get().cycles_to_ms(end - beg);

        //@@NOTE: Use this to know the time needed to update the game (without rendering)
        //LOG_NAMED(ms);

        render();
        post_render();

    }

    auto es = EntityManager::get().serialize_entities();

    write_to_file("lastRunState.json", es.dump(4).c_str());

    shut_down();

    //@@NOTE: Use this to see the console when shutting down
    //ENDL;
    //press_to_continue();
    return 0;
}