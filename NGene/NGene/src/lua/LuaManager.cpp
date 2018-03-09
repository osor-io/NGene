#include "LuaManager.h"



LuaManager::LuaManager() {
    m_state.open_libraries(sol::lib::base);
}


LuaManager::~LuaManager()
{
}

void LuaManager::startUp()
{
}

void LuaManager::
shutDown()
{
}

sol::state & LuaManager::getStateRef() {
    return m_state;
}
