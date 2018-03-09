
#include "./test/testmain.inl"

#include "./_component/ComponentManager.h"
#include "./_entity/EntityManager.h"

void startUp() {
    ComponentManager::get().startUp();
    EntityManager::get().startUp();
}

void shutDown() {
    EntityManager::get().shutDown();
    ComponentManager::get().shutDown();
}

int main() {
    startUp();

    testSystems();

    shutDown();
    return 0;
}