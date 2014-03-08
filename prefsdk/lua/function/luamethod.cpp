#include "luamethod.h"

namespace Lua
{
    LuaMethod::LuaMethod(lua_State *l, lua_String name): LuaReference(l)
    {
        lua_getglobal(l, name);
        this->_ref = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    LuaMethod::LuaMethod(lua_State *l, int index): LuaReference(l, index)
    {
    }

    int LuaMethod::argumentCount() const
    {
        return 0;
    }
}
