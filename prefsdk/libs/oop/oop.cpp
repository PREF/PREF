#include "oop.h"

namespace PrefSDK
{
    QVector<luaL_Reg> LuaOOP::_methods;

    LuaOOP::LuaOOP()
    {

    }

    void LuaOOP::pushMetaTable(lua_State *l, bool hassuper)
    {
        lua_newtable(l);

        if(hassuper)
        {
            lua_pushvalue(l, 1);
            lua_setfield(l, -2, "__index");
        }

        lua_pushcfunction(l, &LuaOOP::metaCall);
        lua_setfield(l, -2, "__call");
    }

    int LuaOOP::metaCall(lua_State *l)
    {
        int argc = lua_gettop(l);

        lua_newtable(l); /* Create self */
        lua_pushvalue(l, 1);
        lua_setmetatable(l, -2);

        lua_getfield(l, -1, "__ctor");

        if(!lua_isnoneornil(l, -1))
        {
            lua_pushvalue(l, -2); /* self */

            for(int i = 2; i <= argc; i++)
                lua_pushvalue(l, i);

            int err = lua_pcall(l, argc, 0, 0);

            if(err)
            {
                throw PrefException(QString::fromUtf8(lua_tostring(l, -1)));
                lua_pop(l, 1);
                return 0;
            }
        }
        else
            lua_pop(l, 1);

        return 1;
    }

    int LuaOOP::metaSuper(lua_State *l)
    {
        int argc = lua_gettop(l);
        lua_getfield(l, lua_upvalueindex(1), "__ctor");

        if(!lua_isnoneornil(l, -1))
        {
            for(int i = 1; i <= argc; i++)
                lua_pushvalue(l, i);

            int err = lua_pcall(l, argc, 0, 0);

            if(err)
            {
                throw PrefException(QString("LuaOOP:metaSuper(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
                lua_pop(l, 1);
            }
        }
        else
            lua_pop(l, 1);

        return 0;
    }

    int LuaOOP::declareClass(lua_State *l)
    {
        int argc = lua_gettop(l);
        bool hassuper = (argc > 0) && (lua_type(l, 1) == LUA_TTABLE);

        lua_newtable(l);

        lua_pushvalue(l, -1);
        lua_setfield(l, -2, "__index");

        if(hassuper)
        {
            lua_pushvalue(l, 1);
            lua_pushcclosure(l, &LuaOOP::metaSuper, 1);
            lua_setfield(l, -2, "__super");
        }

        LuaOOP::pushMetaTable(l, hassuper);
        lua_setmetatable(l, -2);
        return 1;
    }

    void LuaOOP::open(lua_State *l)
    {
        LuaOOP::_methods.append( {"class", &LuaOOP::declareClass} );
        LuaOOP::_methods.append( {nullptr, nullptr} );

        luaL_register(l, "oop", LuaOOP::_methods.cbegin());
        lua_pop(l, 1); /* Pop Table */
    }
}
