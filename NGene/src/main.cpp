#include "./utils/Debug.h"

#include "./time/TimeManager.h"
#include "./render/RenderManager.h"
#include "./debug/AppGUIManager.h"
#include "./lua/LuaManager.h"

#include "./_entity/EntityManager.h"
#include "./_component/ComponentManager.h"
#include "./_system/SystemManager.h"

#include "./_system/systems/RenderSystem.h"

void startUp() {

    TimeManager::get().startUp();
    RenderManager::get().startUp();
    AppGUIManager::get().startUp();
    LuaManager::get().startUp();

    RenderSystem::get().startUp();

    SystemManager::get().startUp();
    ComponentManager::get().startUp();
    EntityManager::get().startUp();

}

void shutDown() {

    EntityManager::get().shutDown();
    ComponentManager::get().shutDown();
    SystemManager::get().shutDown();

    RenderSystem::get().shutDown();

    LuaManager::get().shutDown();
    AppGUIManager::get().shutDown();
    RenderManager::get().shutDown();
    TimeManager::get().shutDown();

}

void loadEntities() {

    LUA.script(R"(
            Entities = {
                Cosa = {
                    TransformComponent = {
                        x = 0,
                        y = 10
                    },
                    PhraseComponent = {
                        phrase = "I'm saying hi from this lua Object!! :D"
                    },
                    SpriteComponent = {
                        filename = "file.png"
                    }
                },
                OtraCosa = {
                    PhraseComponent = {
                        phrase = "I'm saying hi from this lua Object!! :D"
                    },
                    SpriteComponent = {
                        filename = "file.png"
                    }
                },
                YOtraMas = {
                    PhraseComponent = {
                        phrase = "I'm saying hi from this lua Object!! :D"
                    },
                    SpriteComponent = {
                        filename = "file.png"
                    }
                }
            }
        )");

    auto e = EntityManager::get().loadEntity(LUA["Entities"], "Cosa");
    e = EntityManager::get().loadEntity(LUA["Entities"], "OtraCosa");
    e = EntityManager::get().loadEntity(LUA["Entities"], "YOtraMas");
    e = EntityManager::get().loadEntity(LUA["Entities"], "Cosa");
    e = EntityManager::get().loadEntity(LUA["Entities"], "Cosa");
    e = EntityManager::get().loadEntity(LUA["Entities"], "Cosa");
    e = EntityManager::get().loadEntity(LUA["Entities"], "YOtraMas");
    e = EntityManager::get().loadEntity(LUA["Entities"], "Cosa");
    e = EntityManager::get().loadEntity(LUA["Entities"], "Cosa");
    e = EntityManager::get().loadEntity(LUA["Entities"], "Cosa");
    e = EntityManager::get().loadEntity(LUA["Entities"], "Cosa");

    EntityManager::get().removeEntity(e);
    EntityManager::get().removeEntity(5);

    EntityManager::get().updateEntities();
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
            RenderSystem::get().update();
        }
        RenderManager::get().endFrame();
    } // ====== END OF RENDER ======

    EntityManager::get().updateEntities();
}


int main() {
    startUp();
    loadEntities();

    while (RenderManager::get().isWindowOpen()) {
        tick();
    }

    shutDown();

    //ENDL;
    //pressToContinue();
    return 0;
}