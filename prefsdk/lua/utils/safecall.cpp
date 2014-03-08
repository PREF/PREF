#include "safecall.h"

namespace Lua
{
    namespace Utils
    {
        void luaU_safecall(lua_State *l, int argcount, bool hasresult)
        {
            int res = lua_pcall(l, argcount, hasresult ? 1 : 0, 0);

            if(res)
                throw LuaException(lua_tostring(l, -1));
        }
    }
}
