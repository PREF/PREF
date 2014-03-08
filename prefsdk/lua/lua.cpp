#include "lua.h"

namespace Lua
{
    lua_State* luaW_newstate(bool strict)
    {
        lua_State* l = luaL_newstate();
        luaL_openlibs(l);

        if(strict)
            luaL_dostring(l, STRICT_LUA);

        return l;
    }

    void luaW_addsearchpath(lua_State *l, const char *s)
    {
        lua_getglobal(l, "package");
        lua_getfield(l, -1, "path");
        std::string origpath = lua_tostring(l, -1);
        lua_pop(l, 1); /* Pop 'path' */

        std::string newpath = std::string(s) + "/?.lua";
        std::string newsearchpath = origpath +";" + newpath;

        lua_pushstring(l, newsearchpath.c_str());
        lua_setfield(l, -2, "path");
        lua_pop(l, 1); /* Pop 'package' */
    }

    void luaW_dofile(lua_State* l, const char *file)
    {
        int res = luaL_dofile(l, file);

        if(res == 1)
            throw LuaException(lua_tostring(l, -1));
    }

    void luaW_dostring(lua_State* l, const char* s)
    {
        int res = luaL_dostring(l, s);

        if(res == 1)
            throw LuaException(lua_tostring(l, -1));
    }
}
