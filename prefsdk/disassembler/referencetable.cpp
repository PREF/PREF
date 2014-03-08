#include "referencetable.h"

namespace PrefSDK
{
    ReferenceTable::ReferenceTable(lua_State *l, QObject* parent): QObject(parent), LuaCTable(l, "ReferenceTable")
    {
        this->exportMethod<void, ReferenceTable, lua_Integer, lua_Integer, lua_Integer>("codeRef", &ReferenceTable::codeRef);
    }

    bool ReferenceTable::isReference(lua_Integer address)
    {
        return this->_refs.contains(address);
    }

    void ReferenceTable::codeRef(lua_Integer address, lua_Integer addressby, lua_Integer flags)
    {
        Reference::Ptr ref;

        if(this->_refs.contains(address))
        {
            ref = this->_refs[address];
            ref->InstructionAddresses.append(addressby);
        }
        else /* if(!ref) */
        {
            ref = Reference::create();
            ref->Type = ReferenceTable::Code;
            ref->CodeFlags = static_cast<ReferenceTable::CodeReference>(flags);

            if((ref->CodeFlags == ReferenceTable::CallFar) || (ref->CodeFlags == ReferenceTable::CallNear))
                ref->Prefix = "sub_";
            else if((ref->CodeFlags == ReferenceTable::JumpFar) || (ref->CodeFlags == ReferenceTable::JumpNear))
                ref->Prefix = "loc_";

            ref->InstructionAddresses.append(addressby);

            this->_refs[address] = ref;
        }

        emit codeReferenceAdded(address);
    }

    ReferenceTable::Reference::Ptr ReferenceTable::reference(lua_Integer address)
    {
        return this->_refs[address];
    }

    void ReferenceTable::declareGlobals(lua_State *l)
    {
        LuaTable::Ptr t = LuaTable::create(l);
        t->set("CallFar", ReferenceTable::CallFar);
        t->set("CallNear", ReferenceTable::CallNear);
        t->set("JumpFar", ReferenceTable::JumpFar);
        t->set("JumpNear", ReferenceTable::JumpNear);
        t->pushGlobal("CodeReference");

        t = LuaTable::create(l);
        t->set("Offset", ReferenceTable::Offset);
        t->set("Read", ReferenceTable::Read);
        t->set("Write", ReferenceTable::Write);
        t->pushGlobal("DataReference");
    }
}
