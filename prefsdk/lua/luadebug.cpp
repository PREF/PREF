#include "luadebug.h"

namespace Lua
{
    const char* luaD_stackdump(lua_State *l)
    {
        int i = lua_gettop(l);

        if(!i)
            return "The Stack is Empty";

        std::stringstream ss;
        ss << "Stack Size: " << i << std::endl;

        while(i)
        {
            int t = lua_type(l, i);
            ss << i << " -> (" << lua_typename(l, t) << ") ";

            switch(t)
            {
                case LUA_TSTRING:
                    ss << ": '" << lua_tostring(l, i) << "'";
                    break;

                case LUA_TBOOLEAN:
                    ss << ": " << (lua_toboolean(l, i) ? "true" : "false");
                    break;

                case LUA_TNUMBER:
                    ss << ": " << lua_tonumber(l, i);
                    break;

                case LUA_TTABLE:
                {
                    ss << ": Size " << luaT_tablelength(l, i) << std::endl << "Items:" << std::endl;
                    lua_pushnil(l);

                    while(lua_next(l, i))
                    {
                        ss << "  - " << luaT_typevalue(l, -2) << " -> " << luaT_typevalue(l, -1) << std::endl;
                        lua_pop(l, 1);
                    }
                }
                break;

                default:
                    break;
            }

            ss << std::endl;
            i--;
        }

        return strdup(ss.str().c_str());
    }

    const char *luaD_traceback(lua_State *l)
    {
        lua_getglobal(l, "debug");
        lua_getfield(l, -1, "traceback");
        lua_pcall(l, 0, 1, 0);

        const char* s = lua_tostring(l, -1);
        lua_pop(l, 2);
        return s;
    }

}
