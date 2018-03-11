#include "LuaManager.h"



LuaManager::LuaManager() {
}


LuaManager::~LuaManager()
{
}

void LuaManager::startUp()
{
    m_state = std::make_unique<sol::state>();
    m_state.get()->open_libraries(sol::lib::base);
}

void LuaManager::shutDown() {
    m_state.reset();
}

sol::state & LuaManager::getStateRef() {
    return *m_state;
}
