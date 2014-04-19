#ifndef PREFSDK_API_ELEMENTAPI_H
#define PREFSDK_API_ELEMENTAPI_H

#include "prefsdk/format/elements/formatelement.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C" const int ElementType_Invalid = ElementType::Invalid;
        extern "C" const int ElementType_Structure = ElementType::Structure;
        extern "C" const int ElementType_Field = ElementType::Field;
        extern "C" const int ElementType_FieldArray = ElementType::FieldArray;
        extern "C" const int ElementType_BitField = ElementType::BitField;
    }
}

#endif // PREFSDK_API_ELEMENTAPI_H
