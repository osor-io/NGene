#include "LuaManager.h"
#include "./LuaTypes.h"


LuaManager::LuaManager() {
}


LuaManager::~LuaManager()
{
}

void LuaManager::start_up()
{
    m_state = std::make_unique<sol::state>();
    m_state.get()->open_libraries(sol::lib::base);



    LUA.new_usertype<LuaVector2f>("Vector2f",


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "x", &LuaVector2f::x,
        "y", &LuaVector2f::y

        );

}

void LuaManager::shut_down() {
    m_state.reset();
}

sol::state & LuaManager::get_state_ref() {
    return *m_state;
}
