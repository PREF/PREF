#include "processordefinition.h"

namespace PrefSDK
{
    ProcessorDefinition::ProcessorDefinition(lua_State *l): LuaTable(l)
    {

    }

    void ProcessorDefinition::initializePc(lua_Integer basevalue)
    {
        this->setPc(basevalue);
    }

    void ProcessorDefinition::setPc(lua_Integer value)
    {
        this->set("pc", value);
    }

    Instruction::InstructionFeatures ProcessorDefinition::features(lua_Integer itype)
    {
        LuaTable::Ptr itypetable = this->get("features");
        lua_Integer f = itypetable->get<lua_Integer, lua_Integer>(itype);
        return static_cast<Instruction::InstructionFeatures>(f);
    }

    lua_Integer ProcessorDefinition::analyze(const Instruction::Ptr &instr)
    {
        return this->call<lua_Integer, LuaTable::Ptr>("analyze", *instr);
    }

    void ProcessorDefinition::emulate(const AddressQueue::Ptr& queue, ReferenceTable *reftable, const Instruction::Ptr &instr)
    {
        this->call<void, LuaTable::Ptr, LuaTable::Ptr, LuaTable::Ptr>("emulate", *queue, *reftable, *instr);
    }

    QString ProcessorDefinition::output(ReferenceTable *reftable, const Instruction::Ptr &instr)
    {
        OutputBuffer ob(this->state(), reftable);
        this->call<void, LuaTable::Ptr, LuaTable::Ptr>("output", ob, *instr);
        return ob.buffer();
    }

    void ProcessorDefinition::updatePc(lua_Integer amount)
    {
        lua_Integer pc = this->get<lua_String, lua_Integer>("pc");
        this->set("pc", pc + amount);
    }
}
