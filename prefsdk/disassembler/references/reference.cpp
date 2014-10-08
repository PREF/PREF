#include "reference.h"

namespace PrefSDK
{
    Reference::Reference(const DataValue& referencedby, Reference::Type referencetype, QObject *parent): QObject(parent), _referencedby(referencedby), _referencetype(referencetype)
    {

    }

    const DataValue &Reference::referencedBy() const
    {
        return this->_referencedby;
    }

    Reference::Type Reference::type() const
    {
        return this->_referencetype;
    }

    bool Reference::isCodeReference(Reference::Type referencetype)
    {
        return referencetype & Reference::Code;
    }

    bool Reference::isDataReference(Reference::Type referencetype)
    {
        return referencetype & Reference::Data;
    }

    bool Reference::isCall(Reference::Type referencetype)
    {
        return Reference::isCodeReference(referencetype) && ((referencetype == Reference::Call) || (referencetype == Reference::ConditionalCall));
    }

    bool Reference::isJump(Reference::Type referencetype)
    {
        return Reference::isCodeReference(referencetype) && ((referencetype == Reference::Jump) || (referencetype == Reference::ConditionalJump));
    }

    bool Reference::isConditional(Reference::Type referencetype)
    {
        return Reference::isCodeReference(referencetype) && (referencetype & Reference::Conditional);
    }
}
