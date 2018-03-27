#include "LuaManager.h"
#include "./LuaTypes.h"

#include "../_entity/EntityManager.h"
#include "../input/InputManager.h"
#include "../time/TimeManager.h"


LuaManager::LuaManager() {
}


LuaManager::~LuaManager()
{
}

void LuaManager::start_up()
{
    reset_state();
}

void LuaManager::shut_down() {
    m_state.reset();
}

sol::state & LuaManager::get_state_ref() {
    return *m_state;
}

void LuaManager::reset_state(){

    if (m_state) {
        m_state.reset();
    }

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

    /*
    @@NOTE
    
    If we are exposing things from managers or systems to Lua, we should expose them
    again here since we are resetting & reloading the full Lua state.
    */
    EntityManager::get().set_scripts();
    InputManager::get().expose_to_lua();
    TimeManager::get().expose_to_lua();

}
