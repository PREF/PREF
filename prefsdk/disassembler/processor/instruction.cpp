#include "instruction.h"

namespace PrefSDK
{
    Instruction::Instruction(lua_State *l, lua_Integer virtualaddress, lua_Integer address, ByteBuffer* bytebuffer): LuaCTable(l, "Instruction")
    {
        this->_type = 0;
        this->_datatype = 0;
        this->_size = 0u;
        this->_virtualaddress = virtualaddress;
        this->_address = address;
        this->_bytebuffer = bytebuffer;

        this->_operands.Op1 = Operand::create(l);
        this->_operands.Op2 = Operand::create(l);
        this->_operands.Op3 = Operand::create(l);
        this->_operands.Op4 = Operand::create(l);
        this->_operands.Op5 = Operand::create(l);
        this->_operands.Op6 = Operand::create(l);

        this->exportField("operand1", reinterpret_cast<LuaCTable::Ptr*>(&this->_operands.Op1));
        this->exportField("operand2", reinterpret_cast<LuaCTable::Ptr*>(&this->_operands.Op2));
        this->exportField("operand3", reinterpret_cast<LuaCTable::Ptr*>(&this->_operands.Op3));
        this->exportField("operand4", reinterpret_cast<LuaCTable::Ptr*>(&this->_operands.Op4));
        this->exportField("operand5", reinterpret_cast<LuaCTable::Ptr*>(&this->_operands.Op5));
        this->exportField("operand6", reinterpret_cast<LuaCTable::Ptr*>(&this->_operands.Op6));
        this->exportField("type", &this->_type);
        this->exportField("datatype", &this->_datatype);
        this->exportField("userparams", &this->_userparams);
        this->exportMethod<lua_Integer, Instruction, lua_Integer>("next", &Instruction::next);
        this->exportMethod<lua_Integer, Instruction>("virtualAddress", &Instruction::virtualAddress);
        this->exportMethod<lua_Integer, Instruction>("address", &Instruction::address);
        this->exportMethod<lua_Integer, Instruction>("size", &Instruction::instructionSize);
    }

    lua_Integer Instruction::virtualAddress()
    {
        return this->_virtualaddress;
    }

    lua_Integer Instruction::address()
    {
        return this->_address;
    }

    lua_Integer Instruction::instructionType()
    {
        return this->_type;
    }

    lua_Integer Instruction::instructionSize()
    {
        return this->_size;
    }

    Operand::Ptr Instruction::operand1()
    {
        return this->_operands.Op1;
    }

    Operand::Ptr Instruction::operand2()
    {
        return this->_operands.Op2;
    }

    Operand::Ptr Instruction::operand3()
    {
        return this->_operands.Op3;
    }

    Operand::Ptr Instruction::operand4()
    {
        return this->_operands.Op4;
    }

    Operand::Ptr Instruction::operand5()
    {
        return this->_operands.Op5;
    }

    Operand::Ptr Instruction::operand6()
    {
        return this->_operands.Op6;
    }

    void Instruction::declareGlobals(lua_State *l)
    {
        LuaTable::Ptr t = LuaTable::create(l);

        t->set("Stop", Instruction::Stop);
        t->set("Call", Instruction::Call);
        t->set("Change1", Instruction::Change1);
        t->set("Change2", Instruction::Change2);
        t->set("Change3", Instruction::Change3);
        t->set("Change4", Instruction::Change4);
        t->set("Change5", Instruction::Change5);
        t->set("Change6", Instruction::Change6);
        t->set("Use1", Instruction::Use1);
        t->set("Use2", Instruction::Use2);
        t->set("Use3", Instruction::Use3);
        t->set("Use4", Instruction::Use4);
        t->set("Use5", Instruction::Use5);
        t->set("Use6", Instruction::Use6);
        t->set("Jump", Instruction::Jump);
        t->set("Shift", Instruction::Shift);

        t->pushGlobal("InstructionFeatures");
    }

    Instruction::Ptr Instruction::create(lua_State *l, lua_Integer virtualaddress, lua_Integer address, ByteBuffer *bytebuffer)
    {
        return Instruction::Ptr(new Instruction(l, virtualaddress, address, bytebuffer));
    }

    lua_Integer Instruction::next(lua_Integer datatype)
    {        
        lua_Integer res = this->_bytebuffer->readType(this->_address + this->_size, datatype);
        this->_size += DataType::sizeOf(static_cast<DataType::Type>(datatype));
        return res;
    }
}
