#ifndef LUATYPES_H
#define LUATYPES_H

#define lua_private private
#define lua_public public

#include <cstring>
#include <sstream>
#include <tuple>
#include "luatypedefs.h"

namespace Lua
{
    typedef void* lua_LightUserData;
    typedef const char* lua_String;

    template<typename T> class ReturnValue
    {
        public:
            ReturnValue(T t): _t(t) { }
            ReturnValue(const ReturnValue<T>& rt): _t(rt._t) { }

            operator T()
            {
                return this->_t;
            }

            ReturnValue<T>& operator=(const ReturnValue<T>& rhs)
            {
                if(this == &rhs)
                    return *this;

                this->_t = rhs._t;
                return *this;
            }

            ReturnValue<T>& operator=(const T& rhs)
            {
                this->_t = rhs;
            }

        private:
            T _t;
    };

    template<> class ReturnValue<void>
    {
        public:
            ReturnValue() { }
            ReturnValue(const ReturnValue<void>&) { }
            operator void() { }
            ReturnValue<void>& operator=(const ReturnValue<void>&) { return *this; }
    };

    template<> struct TypeOf<lua_Integer>
    {
        typedef lua_Integer Type;
        static constexpr LuaTypes::LuaType LuaType = LuaTypes::Integer;
        static constexpr const char* TypeName = "Integer";
    };

    template<> struct TypeOf<lua_Number>
    {
        typedef lua_Number Type;
        static constexpr LuaTypes::LuaType LuaType = LuaTypes::Number;
        static constexpr const char* TypeName = "Number";
    };

    template<> struct TypeOf<lua_String>
    {
        typedef lua_String Type;
        static constexpr LuaTypes::LuaType LuaType = LuaTypes::String;
        static constexpr const char* TypeName = "String";
    };

    template<> struct TypeOf<lua_LightUserData>
    {
        typedef lua_LightUserData Type;
        static constexpr LuaTypes::LuaType LuaType = LuaTypes::UserData;
        static constexpr const char* TypeName = "UserData";
    };

    template<> struct TypeOf<bool>
    {
        typedef bool Type;
        static constexpr LuaTypes::LuaType LuaType = LuaTypes::Bool;
        static constexpr const char* TypeName = "Bool";
    };

    template<> struct ValueExtractor<lua_Integer>
    {
        static lua_Integer get(lua_State *l)
        {
            lua_Integer val = lua_tointeger(l, -1);
            lua_pop(l, 1);
            return val;
        }
    };

    template<> struct ValueExtractor<lua_Number>
    {
        static lua_Number get(lua_State *l)
        {
            lua_Number val = lua_tonumber(l, -1);
            lua_pop(l, 1);
            return val;
        }
    };

    template<> struct ValueExtractor<lua_String>
    {
        static lua_String get(lua_State *l)
        {
            lua_String val = lua_tostring(l, -1);
            lua_pop(l, 1);
            return val;
        }
    };

    template<> struct ValueExtractor<lua_LightUserData>
    {
        static lua_LightUserData get(lua_State *l)
        {
            lua_LightUserData val = lua_touserdata(l, -1);
            lua_pop(l, 1);
            return val;
        }
    };

    template<> struct ValueExtractor<bool>
    {
        static bool get(lua_State *l)
        {
            bool val = (lua_toboolean(l, -1) == 1);
            lua_pop(l, 1);
            return val;
        }
    };

    template<LuaTypes::LuaType> struct TypeCreator
    {

    };

    template<> struct TypeCreator<LuaTypes::Integer>
    {
        typedef lua_Integer Type;
    };

    template<> struct TypeCreator<LuaTypes::Number>
    {
        typedef lua_Number Type;
    };

    template<> struct TypeCreator<LuaTypes::String>
    {
        typedef lua_String Type;
    };

    template<> struct TypeCreator<LuaTypes::LightUserData>
    {
        typedef lua_LightUserData Type;
    };

    template<> struct TypeCreator<LuaTypes::Bool>
    {
        typedef bool Type;
    };

    LuaTypes::LuaType luaT_typeof(lua_State* l, int index);
    lua_String luaT_typevalue(lua_State* l, int index);
    lua_String luaT_typename(lua_State* l, int index);
    lua_String luaT_typename(LuaTypes::LuaType t);
    bool luaT_isref(lua_State* l, int index);
    bool luaT_isvalue(lua_State* l, int index);
    int luaT_tablelength(lua_State* l, int index);
    int luaT_pushtype(lua_State* l, int narg);
    int luaT_typeerror(lua_State* l, int narg, const char* tname);

    inline void luaT_getvalue(lua_State* l, int index, lua_Integer& v) { v = lua_tointeger(l, index); }
    inline void luaT_getvalue(lua_State* l, int index, lua_Number& v) { v = lua_tonumber(l, index); }
    inline void luaT_getvalue(lua_State* l, int index, lua_String& v) { v = lua_tostring(l, index); }
    inline void luaT_getvalue(lua_State* l, int index, lua_LightUserData& v) { v = lua_touserdata(l, index); }
    inline void luaT_getvalue(lua_State* l, int index, bool& v) { v = (lua_toboolean(l, index) == 1); }

    inline void luaT_pushvalue(lua_State* l, lua_Integer v) { lua_pushinteger(l, v); }
    inline void luaT_pushvalue(lua_State* l, lua_Number v) { lua_pushnumber(l, v); }
    inline void luaT_pushvalue(lua_State* l, lua_String v) { lua_pushstring(l, v); }
    inline void luaT_pushvalue(lua_State* l, lua_LightUserData v) { lua_pushlightuserdata(l, v); }
    inline void luaT_pushvalue(lua_State* l, bool v) { lua_pushboolean(l, v); }

    template<typename T> inline void luaT_getargument(lua_State* l, int narg, T& t)
    {
        LuaTypes::LuaType t1 = TypeOf<T>::LuaType;
        LuaTypes::LuaType t2 = luaT_typeof(l, narg);

        if(t1 == LuaTypes::Integer) /* Correct the type using Lua Type System */
            t1 = LuaTypes::Number;

        if(t2 != t1)
            luaT_typeerror(l, narg, TypeOf<T>::TypeName);

        luaT_getvalue(l, narg, t);
    }
}
#endif // LUATYPES_H
