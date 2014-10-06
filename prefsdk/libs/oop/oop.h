#ifndef PREFSDK_OOP_H
#define PREFSDK_OOP_H

#include <QtCore>
#include <lua.hpp>
#include "prefsdk/prefexception.h"

namespace PrefSDK
{
    class LuaOOP
    {
        private:
            LuaOOP();

        public:
            static void open(lua_State* l);

        private:
            static void pushMetaTable(lua_State* l, bool hassuper);
            static int metaCall(lua_State* l);
            static int metaSuper(lua_State* l);

        private:
            static int declareClass(lua_State* l);

        private:
            static QVector<luaL_Reg> _methods;
    };
}

#endif // OOP_H
