#ifndef PREFSDK_API_DATATYPEAPI_H
#define PREFSDK_API_DATATYPEAPI_H

#include "prefsdk/datatype.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            /* DataType's Flags */
            const u_int64_t DataType_Invalid = DataType::Invalid;

            const u_int64_t DataType_UInt8 = DataType::UInt8;
            const u_int64_t DataType_UInt16 = DataType::UInt16;
            const u_int64_t DataType_UInt32 = DataType::UInt32;
            const u_int64_t DataType_UInt64 = DataType::UInt64;

            const u_int64_t DataType_Int8 = DataType::Int8;
            const u_int64_t DataType_Int16 = DataType::Int16;
            const u_int64_t DataType_Int32 = DataType::Int32;
            const u_int64_t DataType_Int64 = DataType::Int64;

            const u_int64_t DataType_AsciiCharacter = DataType::AsciiCharacter;
            const u_int64_t DataType_UnicodeCharacter = DataType::UnicodeCharacter;
            const u_int64_t DataType_Character = DataType::Character;

            const u_int64_t DataType_Array = DataType::Array;
            const u_int64_t DataType_AsciiString = DataType::AsciiString;
            const u_int64_t DataType_UnicodeString = DataType::UnicodeString;

            const u_int64_t DataType_Blob = DataType::Blob;

            /* DataType's Functions */
            bool DataType_isInteger(u_int64_t type);
            bool DataType_isSigned(u_int64_t type);
            bool DataType_isString(u_int64_t type);
            bool DataType_isAscii(u_int64_t type);
            bool DataType_isUnicode(u_int64_t type);
            bool DataType_isArray(u_int64_t type);
            int DataType_sizeOf(u_int64_t type);
            int DataType_byteWidth(u_int64_t type);
            const char* DataType_stringValue(u_int64_t type);
        }
    }
}

#endif // DATATYPEAPI_H
