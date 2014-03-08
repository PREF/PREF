#include "luathread.h"

namespace Lua
{
    LuaThread::LuaThread(lua_State* parentstat, lua_State* threadstat, int index): LuaReference(parentstat, index), _threadstate(threadstat)
    {

    }

    LuaThread::~LuaThread()
    {

    }

    LuaThread::operator lua_State *() const
    {
        return this->_threadstate;
    }
}
