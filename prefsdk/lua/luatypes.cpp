#include "luatypes.h"

namespace Lua
{
    LuaTypes::LuaType luaT_typeof(lua_State *l, int index)
    {
        return static_cast<LuaTypes::LuaType>(lua_type(l, index));
    }

    bool luaT_isref(lua_State *l, int index)
    {
        LuaTypes::LuaType t = luaT_typeof(l, index);

        switch(t)
        {
            case LuaTypes::Table:
            case LuaTypes::Function:
            case LuaTypes::Thread:
                return true;

            default:
                break;
        }

        return false;
    }

    bool luaT_isvalue(lua_State *l, int index)
    {
        LuaTypes::LuaType t = luaT_typeof(l, index);

        switch(t)
        {
            case LuaTypes::Nil:
            case LuaTypes::Bool:
            case LuaTypes::LightUserData:
            case LuaTypes::Number:
            case LuaTypes::String:
            case LuaTypes::UserData:
                return true;

            default:
                break;
        }

        return false;
    }

    const char *luaT_typename(LuaTypes::LuaType t)
    {
        switch(t)
        {
            case LuaTypes::Nil:
                return "Nil";

            case LuaTypes::Bool:
                return "Bool";

            case LuaTypes::LightUserData:
                return "LightUserData";

            case LuaTypes::Number:
                return "Number";

            case LuaTypes::String:
                return "String";

            case LuaTypes::Table:
                return "Table";

            case LuaTypes::Function:
                return "Function";

            case LuaTypes::UserData:
                return "UserData";

            case LuaTypes::Thread:
                return "Thread";

            /* Extra Lua Wrapper Types */
            case LuaTypes::Integer:
                return "Integer";

            case LuaTypes::CTable:
                return "CTable";

            default:
                break;
        }

        return "Unknown";
    }

    lua_String luaT_typevalue(lua_State *l, int index)
    {
        std::stringstream ss;
        LuaTypes::LuaType t = luaT_typeof(l, index);

        switch(t)
        {
            case LuaTypes::Number:
                ss << lua_tonumber(l, index);
                break;

            case LuaTypes::String:
                ss << lua_tostring(l, index);
                break;

            case LuaTypes::Bool:
                ss << (lua_toboolean(l, index) != 0 ? "true" : "false");
                break;

            case LuaTypes::LightUserData:
            case LuaTypes::UserData:
                ss << std::hex << lua_touserdata(l, index);
                break;

            case LuaTypes::Nil:
                ss << "nil";
                break;

            default:
                ss << luaT_typename(t);
                break;
        }

        return strdup(ss.str().c_str());
    }

    int luaT_tablelength(lua_State *l, int index)
    {
        if(luaT_typeof(l, index) != LuaTypes::Table)
            return 0;

        int len = 0;
        lua_pushvalue(l, index);
        lua_pushnil(l);

        while(lua_next(l, -2))
        {
            lua_pop(l, 1);
            len++;
        }

        lua_pop(l, 1);
        return len;
    }

    lua_String luaT_typename(lua_State *l, int index)
    {
        return luaT_typename(luaT_typeof(l, index));
    }

    int luaT_pushtype(lua_State *l, int narg)
    {
        if(!luaL_callmeta(l, narg, "__type"))
            lua_pushstring(l, luaL_typename(l, narg));

        return 1;
    }

    int luaT_typeerror(lua_State *l, int narg, const char *tname)
    {
        const char *msg;
        luaT_pushtype(l, narg);
        msg = lua_pushfstring(l, "%s expected, got %s", tname, lua_tostring(l, -1));
        return luaL_argerror(l, narg, msg);
    }

}
