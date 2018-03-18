#include "./utils/Debug.h"
#include "./File.h"

#include "./debug/LoggingManager.h"
#include "./resources/TextFileManager.h"
#include "./resources/TextureManager.h"
#include "./input/InputManager.h"
#include "./time/TimeManager.h"
#include "./window/WindowManager.h"
#include "./render/RenderManager.h"
#include "./debug/AppGUIManager.h"
#include "./lua/LuaManager.h"

#include "./_entity/EntityManager.h"
#include "./_component/ComponentManager.h"
#include "./_system/SystemManager.h"

#include "./_system/systems/InputSystem.h"
#include "./_system/systems/BehaviourSystem.h"
#include "./_system/systems/RenderSystem.h"

void start_up() {

    LoggingManager::get().start_up();
    TextFileManager::get().start_up();
    TextureManager::get().start_up();
    LuaManager::get().start_up();
    TimeManager::get().start_up();
    WindowManager::get().start_up();
    RenderManager::get().start_up();
    AppGUIManager::get().start_up();
    InputManager::get().start_up();

    SystemManager::get().start_up();
    ComponentManager::get().start_up();
    EntityManager::get().start_up();

    InputSystem::get().start_up();
    BehaviourSystem::get().start_up();
    RenderSystem::get().start_up();

}

void shut_down() {

    RenderSystem::get().shut_down();
    BehaviourSystem::get().shut_down();
    InputSystem::get().shut_down();

    EntityManager::get().shut_down();
    ComponentManager::get().shut_down();
    SystemManager::get().shut_down();

    InputManager::get().shut_down();
    AppGUIManager::get().shut_down();
    RenderManager::get().shut_down();
    WindowManager::get().shut_down();
    TimeManager::get().shut_down();
    LuaManager::get().shut_down();
    TextureManager::get().shut_down();
    TextFileManager::get().shut_down();
    LoggingManager::get().shut_down();
}

void load_default_state() {

    /*
    auto s = TextFileManager::get().get_scoped_resource("res/states/default_state.json");
    auto j = json::parse(*s.resource);
    EntityManager::get().clear_and_load_entities(j);
    */

    auto id = EntityManager::get().request_load_entity("Cosa");
    EntityManager::get().update_entities();
    auto entity = EntityManager::get().get_entity(id);
    entity->set_name("All Mighty Entity");
    entity->get_component<TransformComponent>()->set_position(sf::Vector2f(300.f, 300.f));
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

inline void tick() {

    InputSystem::get().update();
    BehaviourSystem::get().update();


    { // ====== BEG OF RENDER ======
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
    } // ====== END OF RENDER ======

    EntityManager::get().update_entities();
}


int main() {
    start_up();
    load_default_state();


    while (WindowManager::get().is_window_open()) {
        tick();
    }

    auto es = EntityManager::get().serialize_entities();

    write_to_file("lastRunState.json", es.dump(4).c_str());

    shut_down();

    //@@NOTE: Use this to see the console when shutting down
    //ENDL;
    //press_to_continue();
    return 0;
}