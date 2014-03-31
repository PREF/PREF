#include "datatype.h"

namespace PrefSDK
{
    bool DataType::isInteger(DataType::Type type)
    {
        return (type & DataType::Integer) != 0;
    }

    bool DataType::isSigned(DataType::Type type)
    {
        return (type & DataType::Unsigned) == 0;
    }

    bool DataType::isString(DataType::Type type)
    {
        return (type & (DataType::Vector | DataType::Characters)) != 0;
    }

    bool DataType::isAscii(DataType::Type type)
    {
        return (type & (DataType::Characters | DataType::Ascii)) != 0;
    }

    bool DataType::isUnicode(DataType::Type type)
    {
        return (type & (DataType::Characters | DataType::Unicode)) != 0;
    }

    bool DataType::isArray(DataType::Type type)
    {
        return (type & DataType::Vector) != 0;
    }

    int DataType::sizeOf(DataType::Type type)
    {
        if(type & DataType::Vector)
            return 0;

        if((type & DataType::Ascii) || (type == DataType::UInt8) || (type == DataType::Int8) || (type == DataType::Blob)) /* Blob requires 1 byte (used as array element only) */
            return 1;

        if((type & DataType::Unicode) || (type == DataType::UInt16) || (type == DataType::Int16))
            return 2;

        if((type == DataType::UInt32) || (type == DataType::Int32))
            return 4;

        if((type == DataType::UInt64) || (type == DataType::Int64))
            return 8;

        return 0;
    }

    QString DataType::stringValue(DataType::Type type)
    {
        switch(type)
        {
            case DataType::AsciiCharacter:
                return "AsciiChar";

            case DataType::UnicodeCharacter:
                return "UnicodeChar";

            case DataType::UInt8:
                return "UInt8";

            case DataType::Int8:
                return "Int8";

            case DataType::UInt16:
                return "UInt16";

            case DataType::Int16:
                return "Int16";

            case DataType::UInt32:
                return "UInt32";

            case DataType::Int32:
                return "Int32";

            case DataType::UInt64:
                return "UInt64";

            case DataType::Int64:
                return "Int64";

            case DataType::Array:
                return "List";

            case DataType::AsciiString:
                return "AsciiString";

            case DataType::UnicodeString:
                return "UnicodeString";

            case DataType::Blob: /* Blob requires 1 byte (used in array only) */
                return "Blob";

            default:
                break;
        }

        return "Unknown";
    }

    int DataType::byteWidth(DataType::Type type)
    {
        switch(type)
        {
            case DataType::Blob: /* Blob requires 1 byte (used in array only) */
            case DataType::AsciiCharacter:
            case DataType::UInt8:
            case DataType::Int8:
                return 1;

            case DataType::UnicodeCharacter:
            case DataType::UInt16:
            case DataType::Int16:
                return 2;

            case DataType::UInt32:
            case DataType::Int32:
                return 4;

            case DataType::UInt64:
            case DataType::Int64:
                return 8;

            default:
                break;
        }

        return 0;
    }
}
