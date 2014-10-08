#include "referenceset.h"

namespace PrefSDK
{
    ReferenceSet::ReferenceSet(const DataValue &address, QObject *parent): Block(address, parent)
    {
    }

    void ReferenceSet::add(const DataValue& referencedby, Reference::Type referencetype)
    {
        this->_references.insert(new Reference(referencedby, referencetype, this));
    }

    const QSet<Reference *> &ReferenceSet::references() const
    {
        return this->_references;
    }

    QList<Reference *> ReferenceSet::referenceList() const
    {
        return this->_references.toList();
    }

    Block::Type ReferenceSet::blockType() const
    {
        return Block::ReferenceBlock;
    }
}
