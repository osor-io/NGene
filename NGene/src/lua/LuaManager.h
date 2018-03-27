#pragma once
#include <Manager.h>
#include <sol.hpp>
#include <memory>

#define LUA LuaManager::get().get_state_ref()

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

    void start_up() override;
    void shut_down() override;

    sol::state& get_state_ref();

    void reset_state();

private:
    std::unique_ptr<sol::state> m_state{};
};

