#include "reference.h"

namespace PrefSDK
{
    Reference::Reference(uint64_t srcaddress, uint64_t destaddress, ReferenceTypes::Type referencetype, QObject *parent): QObject(parent), _srcaddress(srcaddress), _destaddress(destaddress), _referencetype(referencetype)
    {

    }

    uint64_t Reference::srcAddress() const
    {
        return this->_srcaddress;
    }

    uint64_t Reference::destAddress() const
    {
        return this->_destaddress;
    }

    ReferenceTypes::Type Reference::type() const
    {
        return this->_referencetype;
    }

    QString Reference::displayAddress() const
    {
        return QString("%1").arg(this->_srcaddress, 8, 16, QLatin1Char('0')).toUpper();
    }

    bool Reference::isCodeReference(ReferenceTypes::Type referencetype)
    {
        return referencetype & ReferenceTypes::Code;
    }

    bool Reference::isDataReference(ReferenceTypes::Type referencetype)
    {
        return referencetype & ReferenceTypes::Data;
    }

    bool Reference::isCall(ReferenceTypes::Type referencetype)
    {
        return Reference::isCodeReference(referencetype) && ((referencetype & ReferenceTypes::Call) || (referencetype & ReferenceTypes::ConditionalCall));
    }

    bool Reference::isJump(ReferenceTypes::Type referencetype)
    {
        return Reference::isCodeReference(referencetype) && ((referencetype & ReferenceTypes::Jump) || (referencetype & ReferenceTypes::ConditionalJump));
    }

    bool Reference::isConditional(ReferenceTypes::Type referencetype)
    {
        return Reference::isCodeReference(referencetype) && (referencetype & ReferenceTypes::Conditional);
    }
}
