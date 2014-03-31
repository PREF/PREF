#include "operand.h"

namespace PrefSDK
{
    Operand::Operand(lua_State *l): LuaCTable(l, "Operand")
    {
        this->_optype = Operand::Void;

        this->exportField<lua_Integer>("datatype", &this->_datatype);
        this->exportField<lua_Integer>("type", &this->_optype);
        this->exportField<lua_Integer>("value", &this->_value);
        this->exportField<lua_Integer>("address", &this->_address);
        this->exportField<lua_Integer>("reg", &this->_reg);
        this->exportField<lua_Integer>("phrase", &this->_phrase);
    }

    Operand::Type Operand::operandType()
    {
        return static_cast<Operand::Type>(this->_optype);
    }

    lua_Integer Operand::dataType()
    {
        return this->_datatype;
    }

    lua_Integer Operand::value()
    {
        return this->_value;
    }

    lua_Integer Operand::address()
    {
        return this->_address;
    }

    lua_Integer Operand::reg()
    {
        return this->_reg;
    }

    lua_Integer Operand::phrase()
    {
        return this->_phrase;
    }

    void Operand::declareGlobals(lua_State *l)
    {
        LuaTable::Ptr t = LuaTable::create(l);

        t->set("Void", Operand::Void);
        t->set("Register", Operand::Register);
        t->set("Memory", Operand::Memory);
        t->set("Phrase", Operand::Phrase);
        t->set("Displacement", Operand::Displacement);
        t->set("Immediate", Operand::Immediate);
        t->set("JumpFar", Operand::JumpFar);
        t->set("JumpNear", Operand::JumpNear);
        t->set("CallFar", Operand::CallFar);
        t->set("CallNear", Operand::CallNear);

        t->pushGlobal("OperandTypes");
    }

    Operand::Ptr Operand::create(lua_State *l)
    {
        return Operand::Ptr(new Operand(l));
    }
}
