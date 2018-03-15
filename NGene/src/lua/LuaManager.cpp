#include "LuaManager.h"



LuaManager::LuaManager() {
}


LuaManager::~LuaManager()
{
}

void LuaManager::start_up()
{
    m_state = std::make_unique<sol::state>();
    m_state.get()->open_libraries(sol::lib::base);
}

void LuaManager::shut_down() {
    m_state.reset();
}

sol::state & LuaManager::get_state_ref() {
    return *m_state;
}
