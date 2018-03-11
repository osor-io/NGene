
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

int main() {
    startUp();

    test();

    shutDown();
    pressToContinue();
    return 0;
}