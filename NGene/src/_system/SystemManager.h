#pragma once
#include <vector>
#include <functional>
#include "Manager.h"
#include "./System.h"
#include "../_entity/Entity.h"

class SystemManager : public Manager<SystemManager> {
	using RegisterFuncVector = std::vector< std::function<void(Entity& entity)> >;
    using DeregisterFuncVector = std::vector< std::function<void(EntityId id)> >;

private:
    friend class CRSP <SystemManager>;
    SystemManager();
    ~SystemManager();

public:
    void startUp() override;
    void shutDown() override;

    void addSystemRegisterFunc(std::function<void(Entity& entity)> funct);
    void addSystemDeregisterFunc(std::function<void(EntityId id)> funct);


    void registerEntityInSystems(Entity& entity);
    void deregisterEntityInSystems(EntityId id);

private:
    RegisterFuncVector m_registerFuncs{};
    DeregisterFuncVector m_deregisterFuncs{};
};

