#pragma once
#include <vector>
#include <functional>
#include "Manager.h"
#include "../_entity/Entity.h"

class SystemManager : public Manager<SystemManager> {
	using RegisterFuncVector = std::vector< std::function<void(Entity& entity)> >;
    using DeregisterFuncVector = std::vector< std::function<void(EntityId id)> >;

private:
    friend class CRSP <SystemManager>;
    SystemManager();
    ~SystemManager();

public:
    void start_up() override;
    void shut_down() override;

    void add_system_register_func(std::function<void(Entity& entity)> funct);
    void add_system_deregister_func(std::function<void(EntityId id)> funct);


    void register_entity_in_systems(Entity& entity);
    void deregister_entity_in_systems(EntityId id);

private:
    RegisterFuncVector m_register_funcs{};
    DeregisterFuncVector m_deregister_funcs{};
};

