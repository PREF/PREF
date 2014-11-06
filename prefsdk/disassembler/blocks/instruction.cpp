#include "instruction.h"

namespace PrefSDK
{
    Instruction::Instruction(const QtLua::LuaTable &tc, DataType::Type addresstype, QObject *parent): Block(parent), _instructiontable(tc), _addresstype(addresstype)
    {
        this->_startaddress = DataValue::create(tc.getInteger("address"), addresstype);
        this->_size = DataValue::create(tc.getInteger("size"), addresstype);
    }

    void Instruction::push() const
    {
        this->_instructiontable.push();
    }

    Block::Type Instruction::blockType() const
    {
        return Block::InstructionBlock;
    }

    QString Instruction::mnemonic() const
    {
        return this->_instructiontable.getString("mnemonic");
    }

    bool Instruction::isJump() const
    {
        return this->_instructiontable.getBoolean("isjump");
    }

    bool Instruction::isCall() const
    {
        return this->_instructiontable.getBoolean("iscall");
    }

    bool Instruction::isDestinationValid() const
    {
        return this->_instructiontable.getBoolean("isdestinationvalid");
    }

    DataValue Instruction::destination() const
    {
        lua_Integer dest = this->_instructiontable.getInteger("destination");
        return DataValue::create(dest, this->_addresstype);
    }
}
