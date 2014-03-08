#include "luafunction.h"

namespace Lua
{    
    LuaFunction::LuaFunction(lua_State *l, lua_String name): LuaReference(l)
    {
        lua_getglobal(l, name);
        this->_ref = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    LuaFunction::LuaFunction(lua_State *l, int index): LuaReference(l, index)
    {
    }

    int LuaFunction::argumentCount() const
    {
        return 0;
    }

    void luaT_getvalue(lua_State *l, int index, LuaFunction::Ptr &v)
    {
        v = LuaFunction::Ptr(new LuaFunction(l, index));
    }

    void luaT_pushvalue(lua_State *, const LuaFunction::Ptr& v)
    {
        v->push();
    }
}
