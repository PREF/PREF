#include "luastate.h"

namespace PrefSDK
{
    lua_State* LuaState::_state = nullptr;

    LuaState::LuaState()
    {
    }

    LuaState::LuaState(const LuaState &)
    {

    }

    lua_State *LuaState::instance()
    {
        if(LuaState::_state == nullptr)
            LuaState::_state = luaL_newstate();

        return LuaState::_state;
    }
}
