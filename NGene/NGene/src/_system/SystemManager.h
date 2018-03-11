#pragma once
#include <vector>
#include <functional>
#include "Manager.h"
#include "./System.h"
#include "../_entity/Entity.h"

class SystemManager : public Manager<SystemManager> {
	using SystemVector = std::vector< std::function<void(Entity& entity)> >;
private:
    friend class CRSP <SystemManager>;
    SystemManager();
    ~SystemManager();

public:
    void startUp() override;
    void shutDown() override;

    void registerSystemFunc(std::function<void(Entity& entity)> funct);

    void registerEntityInSystems(Entity& entity);

private:
	SystemVector m_systems{};
};

