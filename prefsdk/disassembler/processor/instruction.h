#ifndef PREFSDK_INSTRUCTION_H
#define PREFSDK_INSTRUCTION_H

#include "prefsdk/datatype.h"
#include "prefsdk/disassembler/processor/operand.h"

namespace PrefSDK
{
    class Instruction
    {
        private:
            struct Operands
            {
                Operand Op1;
                Operand Op2;
                Operand Op3;
                Operand Op4;
                Operand Op5;
                Operand Op6;
            };

        public:
            enum InstructionFeatures { Stop = 0x0001, Call = 0x0002,
                                       Change1 = 0x0004, Change2 = 0x0008, Change3 = 0x0010, Change4 = 0x00020, Change5 = 0x00040, Change6 = 0x00080,
                                       Use1 = 0x00100, Use2 = 0x00200, Use3 = 0x00400, Use4 = 0x00800, Use5 = 0x01000, Use6 = 0x02000,
                                       Jump = 0x04000, Shift = 0x08000 };

        public:
            Instruction(uint64_t address, uint64_t virtualaddress);
            void setType(uint64_t type);
            void updateSize(uint64_t size);
            uint64_t virtualAddress();
            uint64_t address();
            uint64_t instructionType();
            uint64_t instructionSize();
            Operand* operand1();
            Operand* operand2();
            Operand* operand3();
            Operand* operand4();
            Operand* operand5();
            Operand* operand6();

        private:
            uint64_t _size;
            uint64_t _address;
            uint64_t _virtualaddress;
            uint64_t _type;
            DataType::Type _datatype;
            Operands _operands;
    };
}

#endif // PREFSDK_INSTRUCTION_H
