
#include "./test/testmain.inl"

#include "./_component/ComponentManager.h"
#include "./_entity/EntityManager.h"
#include "./_system/SystemManager.h"

#include "./_system/systems/RenderSystem.h"

void startUp() {

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

}

void loadEntities() {
    
        LUA.script(R"(
            Entities = {
                Cosa = {
                    TransformComponent = {
                        x = 0,
                        y = 10
                    },
                    SimplePhraseComponent = {
                        Phrase = "I'm saying hi from this lua Object!! :D"
                    },
                    SimpleGraphicsComponent = {
                        Filename = "file.png"
                    }
                },
                OtraCosa = {
                    SimplePhraseComponent = {
                        Phrase = "I'm saying hi from this lua Object!! :D"
                    },
                    SimpleGraphicsComponent = {
                        Filename = "file.png"
                    }
                },
                YOtraMas = {
                    SimplePhraseComponent = {
                        Phrase = "I'm saying hi from this lua Object!! :D"
                    },
                    SimpleGraphicsComponent = {
                        Filename = "file.png"
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
    
}


void accessEntitiesFromLua() {
    LUA.script(R"(
            print("Do we have an entity with the id 0? ", hasEntity(0))
            print("Do we have an entity with the id 1? ", hasEntity(1))

            print("Loading Entity 0, we expect this one to exist")
            local entity = getEntity(0);
            print("Entity returned: ", entity)
            if(entity) then
                print("The entity existed, lets see if it has a phrase component")
                local phrase = entity:getSimplePhraseComponent()
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

void tickSystems() {
    RenderSystem::get().update();
}


int main() {
    startUp();

    loadEntities();
    accessEntitiesFromLua();
    tickSystems();

    shutDown();
    
    ENDL;
    pressToContinue();
    return 0;
}