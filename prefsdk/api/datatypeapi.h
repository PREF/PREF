#ifndef PREFSDK_API_DATATYPEAPI_H
#define PREFSDK_API_DATATYPEAPI_H

#include <cstdint>
#include "prefsdk/datatype.h"

namespace PrefSDK
{
    namespace API
    {
        /* DataType's Flags */
        extern "C" const uint64_t DataType_Invalid = DataType::Invalid;

        extern "C" const uint64_t DataType_UInt8 = DataType::UInt8;
        extern "C" const uint64_t DataType_UInt16 = DataType::UInt16;
        extern "C" const uint64_t DataType_UInt32 = DataType::UInt32;
        extern "C" const uint64_t DataType_UInt64 = DataType::UInt64;

        extern "C" const uint64_t DataType_Int8 = DataType::Int8;
        extern "C" const uint64_t DataType_Int16 = DataType::Int16;
        extern "C" const uint64_t DataType_Int32 = DataType::Int32;
        extern "C" const uint64_t DataType_Int64 = DataType::Int64;

        extern "C" const uint64_t DataType_AsciiCharacter = DataType::AsciiCharacter;
        extern "C" const uint64_t DataType_UnicodeCharacter = DataType::UnicodeCharacter;
        extern "C" const uint64_t DataType_Character = DataType::Character;

        extern "C" const uint64_t DataType_Array = DataType::Array;
        extern "C" const uint64_t DataType_AsciiString = DataType::AsciiString;
        extern "C" const uint64_t DataType_UnicodeString = DataType::UnicodeString;

        extern "C" const uint64_t DataType_Blob = DataType::Blob;

        extern "C"
        {
            /* DataType's Functions */
            bool DataType_isInteger(uint64_t type);
            bool DataType_isSigned(uint64_t type);
            bool DataType_isString(uint64_t type);
            bool DataType_isAscii(uint64_t type);
            bool DataType_isUnicode(uint64_t type);
            bool DataType_isArray(uint64_t type);
            int DataType_sizeOf(uint64_t type);
            int DataType_byteWidth(uint64_t type);
            const char* DataType_stringValue(uint64_t type);
        }
    }
}

#endif // DATATYPEAPI_H
