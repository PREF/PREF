#include "processordefinition.h"

namespace PrefSDK
{
    ProcessorDefinition::ProcessorDefinition(InstructionSet *instructionset, RegisterSet *registerset, DataType::Type addresstype, QObject *parent): LogObject(parent), _instructionset(instructionset), _registerset(registerset), _addresstype(addresstype)
    {
        this->_instructionset->setParent(this);
        this->_registerset->setParent(this);
    }

    const PrefSDK::QtLua::LuaFunction& ProcessorDefinition::analyze() const
    {
        return this->_analyzefunc;
    }

    const PrefSDK::QtLua::LuaFunction &ProcessorDefinition::emulate() const
    {
        return this->_emulatefunc;
    }

    const PrefSDK::QtLua::LuaFunction &ProcessorDefinition::elaborate() const
    {
        return this->_elaboratefunc;
    }

    void ProcessorDefinition::setAnalyze(const PrefSDK::QtLua::LuaFunction &af)
    {
        this->_analyzefunc = af;
    }

    void ProcessorDefinition::setEmulate(const PrefSDK::QtLua::LuaFunction &ef)
    {
        this->_emulatefunc = ef;
    }

    void ProcessorDefinition::setElaborate(const PrefSDK::QtLua::LuaFunction &ef)
    {
        this->_elaboratefunc = ef;
    }

    lua_Integer ProcessorDefinition::callAnalyze(Instruction *instruction, const DataValue& baseaddress)
    {
        if(!this->_analyzefunc.isValid())
            return 0;

        lua_State* l = this->_analyzefunc.state();
        QtLua::pushObject(l, instruction);
        lua_pushinteger(l, baseaddress.compatibleValue<lua_Integer>());
        bool err = this->_analyzefunc(2, 1, true);

        if(err)
        {
            throw new PrefException(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);
            return 0;
        }

        lua_Integer size = lua_tointeger(l, 1);
        lua_pop(l, 1);
        return size;
    }

    void ProcessorDefinition::callEmulate(ProcessorEmulator* emulator, MemoryBuffer* memorybuffer, Instruction* instruction)
    {
        if(!this->_emulatefunc.isValid())
            return;

        lua_State* l = this->_emulatefunc.state();
        QtLua::pushObject(l, emulator);
        QtLua::pushObject(l, memorybuffer);
        QtLua::pushObject(l, instruction);
        bool err = this->_emulatefunc(3, 0, true);

        if(err)
        {
            throw new PrefException(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);
        }
    }

    void ProcessorDefinition::callElaborate(DisassemblerListing *listing)
    {
        if(!this->_elaboratefunc.isValid())
            return;

        lua_State* l = this->_elaboratefunc.state();
        QtLua::pushObject(l, listing);
        bool err = this->_elaboratefunc(1);

        if(err)
        {
            throw new PrefException(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);
        }
    }

    DataType::Type ProcessorDefinition::addressType() const
    {
        return this->_addresstype;
    }

    PrefSDK::InstructionSet *ProcessorDefinition::instructionSet() const
    {
        return this->_instructionset;
    }

    PrefSDK::RegisterSet *ProcessorDefinition::registerSet() const
    {
        return this->_registerset;
    }

    bool ProcessorDefinition::decode(Instruction *instruction)
    {
        if(!this->_instructionset->isOpcode(instruction->opcodeValue()))
            return false;

        InstructionDefinition* instructiondefinition = this->_instructionset->instructionDefinition(instruction->opcodeValue());
        instruction->setMnemonic(instructiondefinition->mnemonic());
        instruction->setCategory(instructiondefinition->category());
        instruction->setType(instructiondefinition->type());
        return true;
    }
}
