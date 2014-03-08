#ifndef PREFSDK_INSTRUCTION_H
#define PREFSDK_INSTRUCTION_H

#include "prefsdk/qlua.h"
#include "prefsdk/datatype.h"
#include "prefsdk/disassembler/processor/operand.h"
#include "prefsdk/io/bytebuffer.h"

namespace PrefSDK
{
    class Instruction: public LuaCTable
    {
        private:
            struct Operands
            {
                Operand::Ptr Op1;
                Operand::Ptr Op2;
                Operand::Ptr Op3;
                Operand::Ptr Op4;
                Operand::Ptr Op5;
                Operand::Ptr Op6;
            };

        public:
            typedef std::shared_ptr<Instruction> Ptr;
            enum InstructionFeatures { Stop = 0x0001, Call = 0x0002,
                                       Change1 = 0x0004, Change2 = 0x0008, Change3 = 0x0010, Change4 = 0x00020, Change5 = 0x00040, Change6 = 0x00080,
                                       Use1 = 0x00100, Use2 = 0x00200, Use3 = 0x00400, Use4 = 0x00800, Use5 = 0x01000, Use6 = 0x02000,
                                       Jump = 0x04000, Shift = 0x08000 };

        public:
            Instruction(lua_State* l, lua_Integer virtualaddress, lua_Integer address, ByteBuffer* bytebuffer);
            lua_Integer virtualAddress();
            lua_Integer address();
            lua_Integer instructionType();
            lua_Integer instructionSize();
            Operand::Ptr operand1();
            Operand::Ptr operand2();
            Operand::Ptr operand3();
            Operand::Ptr operand4();
            Operand::Ptr operand5();
            Operand::Ptr operand6();

        public:
            static void declareGlobals(lua_State* l);
            static Instruction::Ptr create(lua_State* l, lua_Integer virtualaddress, lua_Integer address, ByteBuffer* bytebuffer);

        lua_public:
            lua_Integer next(lua_Integer datatype);

        private:
            uint _size;
            lua_Integer _virtualaddress;
            lua_Integer _address;
            lua_Integer _type;
            lua_Integer _datatype;
            LuaTable::Ptr _userparams;
            ByteBuffer* _bytebuffer;
            Operands _operands;
    };
}

#endif // PREFSDK_INSTRUCTION_H
