#pragma once

#include "../utils/CRSP.h"
#include "../_entity/Entity.h"
#include "./SystemManager.h"

/**
Base Class for every system in the engine. They should be retrieved
at the initialization phase of the program before the game loop and
call the start_up function in the proper order.

At the end of the program they should be shut_down also in the proper
order. Generally the opposed order to initialization.

*/
template <typename T>
class System : public CRSP<T> {
    friend class CRSP<T>;
protected:
    System() { }
    virtual ~System() { }
public:
    virtual void update() = 0;
    virtual void start_up() = 0;
    virtual void shut_down() = 0;
    virtual void register_entity(Entity& entity) = 0;
    virtual void deregister_entity(EntityId id) = 0;
    void registerSystem() {
        SystemManager::get().add_system_register_func([this](Entity& entity) {
            register_entity(entity);
        });
        SystemManager::get().add_system_deregister_func([this](EntityId id) {
            deregister_entity(id);
        });
    }
};
