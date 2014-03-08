#ifndef PREFSDK_PREFDEBUG_H
#define PREFSDK_PREFDEBUG_H

#include "debugdialog/debugdialog.h"
#include "qlua.h"

namespace PrefSDK
{
    class PrefDebug
    {
        private:
            typedef LuaCFunctionT<void, lua_String> PrintStringFunction;
            typedef LuaCFunctionT<void, lua_Integer> PrintIntegerFunction;
            typedef LuaCFunctionT<void, LuaTable::Ptr> PrintTableFunction;

        public:
            typedef std::shared_ptr<PrefDebug> Ptr;

        public:
            PrefDebug(lua_State* l);
            static PrefDebug::Ptr create(lua_State* l);
            static void dbgprint(QString s);

        public: /* Lua Exported Functions */
            static void dbgprint(lua_String p);
            static void dbgprint(lua_Integer p);
            static void dbgprint(LuaTable::Ptr p);

        private:
            lua_State* _state;

        private:
            static PrintStringFunction::Ptr _printstring;
            static PrintIntegerFunction::Ptr _printinteger;
            static PrintTableFunction::Ptr _printreference;
    };
}

#endif // PREFSDK_PREFDEBUG_H
