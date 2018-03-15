#include "./utils/Debug.h"
#include "./File.h"

#include "./time/TimeManager.h"
#include "./render/RenderManager.h"
#include "./debug/AppGUIManager.h"
#include "./lua/LuaManager.h"

#include "./_entity/EntityManager.h"
#include "./_component/ComponentManager.h"
#include "./_system/SystemManager.h"

#include "./_system/systems/BehaviourSystem.h"
#include "./_system/systems/RenderSystem.h"

void start_up() {

    TimeManager::get().start_up();
    RenderManager::get().start_up();
    AppGUIManager::get().start_up();
    LuaManager::get().start_up();

    BehaviourSystem::get().start_up();
    RenderSystem::get().start_up();

    SystemManager::get().start_up();
    ComponentManager::get().start_up();
    EntityManager::get().start_up();

}

void shut_down() {

    EntityManager::get().shut_down();
    ComponentManager::get().shut_down();
    SystemManager::get().shut_down();

    RenderSystem::get().shut_down();
    BehaviourSystem::get().shut_down();

    LuaManager::get().shut_down();
    AppGUIManager::get().shut_down();
    RenderManager::get().shut_down();
    TimeManager::get().shut_down();

}

void loadEntities() {

    auto e = EntityManager::get().load_entity( "Cosa");
    e = EntityManager::get().load_entity("OtraCosa");
    e = EntityManager::get().load_entity("YOtraMas");
    e = EntityManager::get().load_entity("Cosa");
    e = EntityManager::get().load_entity("Cosa");
    e = EntityManager::get().load_entity("Cosa");
    e = EntityManager::get().load_entity("YOtraMas");
    e = EntityManager::get().load_entity("Cosa");
    e = EntityManager::get().load_entity("Cosa");
    e = EntityManager::get().load_entity("Cosa");
    e = EntityManager::get().load_entity("Cosa");

    EntityManager::get().remove_entity(e);
    EntityManager::get().remove_entity(5);

    EntityManager::get().update_entities();
}


void accessEntitiesFromLua() {
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

    { // ====== BEG OF RENDER ======
        RenderManager::get().beginFrame();
        /*
        Everything that renders something goes in the
        next scope
        */
        {
            BehaviourSystem::get().update();
            RenderSystem::get().update();
        }
        RenderManager::get().endFrame();
    } // ====== END OF RENDER ======

    EntityManager::get().update_entities();
}


int main() {
    start_up();
    loadEntities();


    while (RenderManager::get().isWindowOpen()) {
        tick();
    }

    auto es = EntityManager::get().serialize_entities();
    writeToFile("lastRunState.json", es.dump(4).c_str());


    shut_down();

    //ENDL;
    //pressToContinue();
    return 0;
}