#include "referencetable.h"

namespace PrefSDK
{
    ReferenceTable::ReferenceTable(QObject *parent): QObject(parent)
    {
    }

    uint ReferenceTable::count() const
    {
        return this->_referencetable.count();
    }

    ReferenceSet* ReferenceTable::addReference(const DataValue& address, const DataValue& referencedby, Reference::Type referencetype)
    {
        if(!this->_referencetable.contains(address))
            this->_referencetable[address] = new ReferenceSet(address, this);

        ReferenceSet* refset = this->_referencetable[address];
        refset->add(referencedby, referencetype);
        return refset;
    }

    bool ReferenceTable::isReferenced(Block *block)
    {
        return this->isReferenced(block->startAddress());
    }

    bool ReferenceTable::isReferenced(const DataValue &address)
    {
        return this->_referencetable.contains(address);
    }

    ReferenceSet *ReferenceTable::references(Block *block)
    {
        return this->references(block->startAddress());
    }

    ReferenceSet *ReferenceTable::references(const DataValue &address)
    {
        if(this->_referencetable.contains(address))
            return this->_referencetable[address];

        return nullptr;
    }
}
