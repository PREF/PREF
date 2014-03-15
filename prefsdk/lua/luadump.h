#ifndef LUADUMP_H
#define LUADUMP_H

#include <lua.hpp>
#include <iostream>

namespace Lua
{
    using namespace std;

    void luaW_dumpstack(lua_State* l);
}

#endif // LUADUMP_H
