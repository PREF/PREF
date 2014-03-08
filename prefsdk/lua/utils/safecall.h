#ifndef SAFECALL_H
#define SAFECALL_H

#include <lua.hpp>
#include "../luaexception.h"

namespace Lua
{
    namespace Utils
    {
        void luaU_safecall(lua_State* l, int argcount, bool hasresult);
    }
}

#endif // SAFECALL_H
