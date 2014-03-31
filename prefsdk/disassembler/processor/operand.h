#ifndef PREFSDK_OPERAND_H
#define PREFSDK_OPERAND_H

#include "prefsdk/qlua.h"
#include "prefsdk/datatype.h"

namespace PrefSDK
{
    class Operand : public LuaCTable
    {
        public:
            typedef std::shared_ptr<Operand> Ptr;

        public:
            enum Type { Void, Register, Memory, Phrase, Displacement, Immediate, JumpFar, JumpNear, CallFar, CallNear };
            Operand(lua_State* l);
            Operand::Type operandType();
            lua_Integer dataType();
            lua_Integer value();
            lua_Integer address();
            lua_Integer reg();
            lua_Integer phrase();

        public:
            static void declareGlobals(lua_State* l);
            static Operand::Ptr create(lua_State* l);

        private:
            lua_Integer _optype;
            lua_Integer _datatype;
            lua_Integer _value;
            lua_Integer _address;
            lua_Integer _reg;
            lua_Integer _phrase;
    };
}

#endif // PREFSDK_OPERAND_H
