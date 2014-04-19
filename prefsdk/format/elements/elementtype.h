#ifndef PREFSDK_ELEMENTTYPE_H
#define PREFSDK_ELEMENTTYPE_H

namespace PrefSDK
{
    namespace ElementType
    {
        enum Type
        {
            Invalid    = 0,
            Structure  = 1,
            Field      = 2,
            FieldArray = 3,
            BitField   = 4,
        };
    }
}

#endif // ELEMENTTYPE_H
