#ifndef QLUA_H
#define QLUA_H

#include <QtCore>
#include "lua/luatypedefs.h"

namespace Lua
{
    template<> struct ValueExtractor<QString>
    {
        static QString get(lua_State *l)
        {
            QString val = QString::fromLatin1(lua_tostring(l, -1));
            lua_pop(l, 1);
            return val;
        }
    };

    template<> struct TypeOf<QString>
    {
        typedef QString Type;
        static constexpr LuaTypes::LuaType LuaType = LuaTypes::String;
        static constexpr const char* TypeName = "String";
    };

    inline void luaT_getvalue(lua_State* l, int index, QString& v) { v = QString::fromUtf8(lua_tostring(l, index)); }
    inline void luaT_pushvalue(lua_State* l, QString& v) { lua_pushstring(l, v.toUtf8().constData()); }
}

#include "lua/lua.h"

using namespace Lua;
#endif // QLUA_H
