#ifndef LUA_LUATHREAD_H
#define LUA_LUATHREAD_H

#include "luareference.h"

namespace Lua
{
    class LuaThread: public LuaReference
    {
        public:
            typedef std::shared_ptr<LuaThread> Ptr;

        public:
            LuaThread(lua_State* parentstat, lua_State* threadstat, int index);
            virtual ~LuaThread();
            operator lua_State*() const;

        private:
            lua_State* _threadstate;
    };
}

#endif // LUA_LUATHREAD_H
