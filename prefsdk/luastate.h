#ifndef PREFSDK_LUASTATE_H
#define PREFSDK_LUASTATE_H

#include "lua.hpp"

namespace PrefSDK
{
    class LuaState
    {
        private:
            LuaState();
            LuaState(const LuaState&);

        public:
            static lua_State* instance();

        private:
            static lua_State* _state;
    };
}

#endif // PREFSDK_LUASTATE_H
