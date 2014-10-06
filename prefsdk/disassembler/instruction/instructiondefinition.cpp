#include "instructiondefinition.h"

namespace PrefSDK
{
    InstructionDefinition::InstructionDefinition(const DataValue &opcode, const QString &mnemonic, InstructionCategory::Category category, InstructionType::Type type, QObject *parent): QObject(parent), _opcode(opcode), _mnemonic(mnemonic), _category(category), _type(type)
    {
    }

    lua_Integer InstructionDefinition::opcode() const
    {
        return this->_opcode.compatibleValue<lua_Integer>();
    }

    const QString &InstructionDefinition::mnemonic() const
    {
        return this->_mnemonic;
    }

    lua_Integer InstructionDefinition::category() const
    {
        return this->_category;
    }

    lua_Integer InstructionDefinition::type() const
    {
        return this->_type;
    }
}
