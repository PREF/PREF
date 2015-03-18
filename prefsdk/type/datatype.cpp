#include "datatype.h"

namespace PrefSDK
{
    DataType::DataType(QObject *parent): QObject(parent)
    {

    }

    bool DataType::isInteger(DataType::Type type)
    {
        return (type & DataType::Integer) != 0;
    }

    bool DataType::isSigned(DataType::Type type)
    {
        return DataType::isInteger(type) && (type & DataType::Signed);
    }

    bool DataType::isString(DataType::Type type)
    {
        return (type & DataType::Vector) && (type & DataType::Characters);
    }

    bool DataType::isAscii(DataType::Type type)
    {
        return DataType::isString(type) && (type & DataType::Ascii);
    }

    bool DataType::isUnicode(DataType::Type type)
    {
        return DataType::isString(type) && (type & DataType::Unicode);
    }

    bool DataType::isArray(DataType::Type type)
    {
        return (type & DataType::Vector);
    }

    bool DataType::isLittleEndian(DataType::Type type)
    {
        return DataType::isInteger(type) && (type & DataType::LittleEndian);
    }

    bool DataType::isBigEndian(DataType::Type type)
    {
        return DataType::isInteger(type) && (type & DataType::BigEndian);
    }

    uint DataType::sizeOf(DataType::Type type)
    {
        return DataType::bitWidth(type) / 8;
    }

    uint DataType::byteWidth(DataType::Type type)
    {
        return DataType::sizeOf(type) * 2;
    }

    QSysInfo::Endian DataType::byteOrder(DataType::Type type)
    {
        if(DataType::isInteger(type))
        {
            if(type & DataType::LittleEndian)
                return QSysInfo::LittleEndian;

            return QSysInfo::BigEndian;
        }

        return QSysInfo::ByteOrder;
    }

    DataType::Type DataType::adjust(DataType::Type type, QSysInfo::Endian byteorder)
    {
        switch(DataType::bitWidth(type))
        {
            case 16:
            {
                if(DataType::isSigned(type))
                {
                    if(byteorder == QSysInfo::LittleEndian)
                        return DataType::Int16_LE;

                    return DataType::Int16_BE;
                }

                if(byteorder == QSysInfo::LittleEndian)
                    return DataType::UInt16_LE;

                return DataType::UInt16_BE;
            }

            case 32:
            {
                if(DataType::isSigned(type))
                {
                    if(byteorder == QSysInfo::LittleEndian)
                        return DataType::Int32_LE;

                    return DataType::Int32_BE;
                }

                if(byteorder == QSysInfo::LittleEndian)
                    return DataType::UInt32_LE;

                return DataType::UInt32_BE;
            }

            case 64:
            {
                if(DataType::isSigned(type))
                {
                    if(byteorder == QSysInfo::LittleEndian)
                        return DataType::Int64_LE;

                    return DataType::Int64_BE;
                }

                if(byteorder == QSysInfo::LittleEndian)
                    return DataType::UInt64_LE;

                return DataType::UInt64_BE;
            }

            default:
                break;
        }

        return type;
    }

    DataType::Type DataType::bestType(int bytewidth, bool signedtype)
    {
        int bitwidth = bytewidth * 8;

        if(bitwidth <= 8)
        {
            if(signedtype)
                return DataType::Int8;

            return DataType::UInt8;
        }

        if(bitwidth <= 16)
        {
            if(signedtype)
                return DataType::Int16;

            return DataType::UInt16;
        }

        if(bitwidth <= 32)
        {
            if(signedtype)
                return DataType::Int32;

            return DataType::UInt32;
        }

        if(bitwidth <= 64)
        {
            if(signedtype)
                return DataType::Int64;

            return DataType::UInt64;
        }

        return DataType::Invalid;
    }

    QString DataType::stringValue(DataType::Type type)
    {
        switch(type)
        {
            case DataType::AsciiCharacter:
                return "AsciiChar";

            case DataType::UnicodeCharacter:
                return "UnicodeChar";

            case DataType::AsciiString:
                return "AsciiString";

            case DataType::UnicodeString:
                return "UnicodeString";

            case DataType::Array:
                return "Array";

            case DataType::Blob: /* Blob requires 1 byte (used in array only) */
                return "Blob";

            case DataType::UInt8:
                return "UInt8";

            case DataType::UInt16:
            case DataType::UInt16_LE:
            case DataType::UInt16_BE:
                return "UInt16";

            case DataType::UInt32:
            case DataType::UInt32_LE:
            case DataType::UInt32_BE:
                return "UInt32";

            case DataType::UInt64:
            case DataType::UInt64_LE:
            case DataType::UInt64_BE:
                return "UInt64";

            case DataType::Int8:
                return "Int8";

            case DataType::Int16:
            case DataType::Int16_LE:
            case DataType::Int16_BE:
                return "Int16";

            case DataType::Int32:
            case DataType::Int32_LE:
            case DataType::Int32_BE:
                return "Int32";

            case DataType::Int64:
            case DataType::Int64_LE:
            case DataType::Int64_BE:
                return "Int64";

            default:
                break;
        }

        return "Unknown";
    }

    uint DataType::bitWidth(DataType::Type type)
    {
        switch(type)
        {
            case DataType::Blob: /* Blob requires 1 byte (used in array only) */
            case DataType::AsciiCharacter:
            case DataType::UInt8:
            case DataType::Int8:
                return 8;

            case DataType::UnicodeCharacter:
            case DataType::UInt16:
            case DataType::UInt16_LE:
            case DataType::UInt16_BE:
            case DataType::Int16:
            case DataType::Int16_LE:
            case DataType::Int16_BE:
                return 16;

            case DataType::UInt32:
            case DataType::UInt32_LE:
            case DataType::UInt32_BE:
            case DataType::Int32:
            case DataType::Int32_LE:
            case DataType::Int32_BE:
                return 32;

            case DataType::UInt64:
            case DataType::UInt64_LE:
            case DataType::UInt64_BE:
            case DataType::Int64:
            case DataType::Int64_LE:
            case DataType::Int64_BE:
                return 64;

            default:
                break;
        }

        return 0;
    }
}
