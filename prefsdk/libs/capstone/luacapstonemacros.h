#ifndef LUACAPSTONEMACROS_H
#define LUACAPSTONEMACROS_H

#include <lua.hpp>
#include <capstone.h>
#include "reflection.h"

#define LUACAPSTONE_LIBNAME "capstone"

#define TYPE_ERROR(i, t, expect)  lua_pushfstring(l, "Argument %d must be a %s, %s given", i, t, expect); lua_error(l);
#define EXPECT_MIN_ARGC(argc, c) if(argc < c) { lua_pushfstring(l, "Expected at least %d arguments, %d given", argc, c); lua_error(l); }
#define EXPECT_ARGC(argc, c) if(argc != c) { lua_pushfstring(l, "Expected %d arguments, %d given", argc, c); lua_error(l); }
#define EXPECT_TYPE(type, i) if(lua_type(l, i) != type) { TYPE_ERROR(i, lua_typename(l, type), luaL_typename(l, i)); }

#define VALIDATE_FIELD(name, field) if(!ContainsField(name, field)) { \
                                     lua_pushfstring(l, "Field '%s' not found", field); \
                                     lua_error(l); \
                                     return 0; \
                                    }

#define NOT_IMPLEMENTED(field) lua_pushfstring(l, "Field '%s' not implemented", field); \
                               lua_error(l); \
                               return 0;

template<typename T> void lua_pushcstruct(lua_State* l, T* t, lua_CFunction metaindex, const char* metatable)
{
    T** pt = (T**)lua_newuserdata(l, sizeof(T**));
    *pt = t;

    if(luaL_newmetatable(l, metatable))
    {
        lua_pushcfunction(l, metaindex);
        lua_setfield(l, -2, "__index");
    }

    lua_setmetatable(l, -2);
}

template<typename T> void lua_pushcarray(lua_State* l, T* array, lua_Integer size)
{
    lua_newtable(l);

    for(int i = 0; i < size; i++)
    {
        lua_pushinteger(l, array[i]);
        lua_rawseti(l, -2, i);
    }
}

#endif // LUACAPSTONEMACROS_H

