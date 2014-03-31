#ifndef PREFSDK_API_ELEMENTAPI_H
#define PREFSDK_API_ELEMENTAPI_H

#include "prefsdk/format/elements.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            const int ElementType_Invalid = ElementType::Invalid;
            const int ElementType_Structure = ElementType::Structure;
            const int ElementType_Field = ElementType::Field;
            const int ElementType_FieldArray = ElementType::FieldArray;
            const int ElementType_BitField = ElementType::BitField;
        }
    }
}

#endif // PREFSDK_API_ELEMENTAPI_H
