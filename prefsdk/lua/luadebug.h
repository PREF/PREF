#ifndef LUADEBUG_H
#define LUADEBUG_H

#include <cstring>
#include <sstream>
#include <lua.hpp>
#include "luatypes.h"

namespace Lua
{
    const char *luaD_stackdump(lua_State* l);
    const char* luaD_traceback(lua_State* l);
}

#endif // LUADEBUG_H
