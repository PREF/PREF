#include "instructionset.h"

namespace PrefSDK
{
    InstructionSet::InstructionSet(DataType::Type opcodetype, QObject *parent): QObject(parent), _opcodetype(opcodetype)
    {
    }

    InstructionDefinition *InstructionSet::instructionDefinition(const DataValue& opcode) const
    {
        return this->_opcodes[opcode];
    }

    DataType::Type InstructionSet::opcodeType() const
    {
        return this->_opcodetype;
    }

    bool InstructionSet::isOpcode(const DataValue& opcode) const
    {
        return this->_opcodes.contains(opcode);
    }

    bool InstructionSet::getField(lua_State *l, int idx, const DataValue& opcode, const char *field, int type)
    {
        lua_getfield(l, idx, field);
        int t = lua_type(l, -1);

        if(t != type)
        {
            throw PrefException(QString("InstructionSet::getField(): '%1' field expected for opcode %2h").arg(QString::fromUtf8(field), opcode.toString(16)));
            lua_pop(l, 1);
            return false;
        }

        return true;
    }

    int InstructionSet::metaIndex(lua_State *l, lua_Integer key)
    {
        DataValue opcode = DataValue::create(key, this->_opcodetype);

        if(this->_opcodes.contains(opcode))
        {
            QtLua::pushObject(l, this->_opcodes[opcode]);
            return 1;
        }

        throw PrefException(QString("InstructionSet::metaIndex(): Invalid opcode: %1h").arg(opcode.toString(16)));
        return 0;
    }

    int InstructionSet::metaIndex(lua_State *l, QString key)
    {
        if(this->_mnemonics.contains(key))
        {
            QtLua::pushObject(l, this->_mnemonics[key]);
            return 1;
        }

        throw PrefException(QString("InstructionSet::metaIndex(): Invalid mnemonic '%1'").arg(key));
        return 0;
    }

    bool InstructionSet::metaNewIndex(lua_State *l, lua_Integer key)
    {
        int t = lua_type(l, 3);

        if(t != LUA_TTABLE)
        {
            throw PrefException(QString("InstructionSet::metaNewIndex(): Expected 'table' type not '%1'").arg(QString::fromUtf8(lua_typename(l, t))));
            return false;
        }

        DataValue opcode = DataValue::create(key, this->_opcodetype);

        /* Get Mnemonic Field */
        if(!this->getField(l, 3, opcode, "mnemonic", LUA_TSTRING))
            return false;

        QString mnemonic = QString::fromUtf8(lua_tostring(l, -1));
        lua_pop(l, 1);

        /* Get Category Field */
        if(!this->getField(l, 3, opcode, "category", LUA_TNUMBER))
            return false;

        InstructionCategory::Category category = static_cast<InstructionCategory::Category>(lua_tointeger(l, -1));
        lua_pop(l, 1);

        /* Get Type Field */
        if(!this->getField(l, 3, opcode, "type", LUA_TNUMBER))
            return false;

        InstructionType::Type type = static_cast<InstructionType::Type>(lua_tointeger(l, -1));
        lua_pop(l, 1);

        InstructionDefinition* instructiondefinition = new InstructionDefinition(opcode, mnemonic, category, type, this);
        this->_opcodes[opcode] = instructiondefinition;
        this->_mnemonics[mnemonic] = instructiondefinition;
        return true;
    }
}
