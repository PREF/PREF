#include "reference.h"

namespace PrefSDK
{
    Reference::Reference(uint64_t address, ReferenceTypes::Type referencetype, QObject *parent): QObject(parent), _address(address), _referencetype(referencetype)
    {

    }

    uint64_t Reference::address() const
    {
        return this->_address;
    }

    ReferenceTypes::Type Reference::type() const
    {
        return this->_referencetype;
    }
}
