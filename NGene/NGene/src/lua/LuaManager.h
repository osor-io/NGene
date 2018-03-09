#pragma once
#include <Manager.h>
#include <sol.hpp>

#define LUA LuaManager::get().getStateRef()

/**
Manages the global lua state for the application. It can be
easily accessed with the macro LUA to directly access a reference
to the state.

@see sol::state
*/
class LuaManager : public Manager<LuaManager> {
    friend class CRSP<LuaManager>;
private:
    LuaManager();
    ~LuaManager();

public:

    void startUp() override;
    void shutDown() override;

    sol::state& getStateRef();
private:
    sol::state m_state{};
};

