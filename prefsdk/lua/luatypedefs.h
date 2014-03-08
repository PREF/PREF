#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <lua.hpp>

namespace Lua
{
    namespace LuaTypes
    {
        enum LuaType
        {
            None          = LUA_TNONE,
            Nil           = LUA_TNIL,
            Bool          = LUA_TBOOLEAN,
            LightUserData = LUA_TLIGHTUSERDATA,
            Number        = LUA_TNUMBER,
            String        = LUA_TSTRING,
            Table         = LUA_TTABLE,
            Function      = LUA_TFUNCTION,
            UserData      = LUA_TUSERDATA,
            Thread        = LUA_TTHREAD,

            Integer       = 0x1000, /* Special Type for Wrapper when exporting Integers from C++ to Lua */
            CTable        = 0x1001  /* Special Type for Wrapper when exporting Tables from C++ to Lua */
        };
    }

    template<typename T> struct ValueExtractor
    {

    };

    template<typename T> struct TypeOf
    {
        typedef T Type;
        static constexpr LuaTypes::LuaType LuaType = LuaTypes::None;
        static constexpr const char* TypeName = "None";
    };
}

#endif // TYPEDEFS_H
