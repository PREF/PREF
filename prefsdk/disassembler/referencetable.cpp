#include "referencetable.h"

namespace PrefSDK
{
    ReferenceTable::ReferenceTable(QObject* parent): QObject(parent)
    {

    }

    bool ReferenceTable::isReference(uint64_t address)
    {
        return this->_refs.contains(address);
    }


    /*
    void ReferenceTable::codeRef(uint64_t address, uint64_t addressby, uint64_t flags)
    {
        Reference ref;

        if(this->_refs.contains(address))
        {
            ref = this->_refs[address];
            ref->InstructionAddresses.append(addressby);
        }
        else if(!ref)
        {
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
    */


    ReferenceTable::Reference ReferenceTable::reference(uint64_t address)
    {
        return this->_refs[address];
    }
}
