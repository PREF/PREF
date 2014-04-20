#ifndef PREFSDK_OPERAND_H
#define PREFSDK_OPERAND_H

#include "prefsdk/datatype.h"

namespace PrefSDK
{
    class Operand
    {
        public:
            enum Type { Void, Register, Memory, Phrase, Displacement, Immediate, JumpFar, JumpNear, CallFar, CallNear };

        public:
            Operand();
            Operand::Type operandType();
            DataType::Type dataType();
            uint64_t value();
            uint64_t address();
            uint64_t reg();
            uint64_t phrase();

        private:
            Operand::Type _optype;
            DataType::Type _datatype;
            uint64_t _value;
            uint64_t _address;
            uint64_t _reg;
            uint64_t _phrase;
    };
}

#endif // PREFSDK_OPERAND_H
