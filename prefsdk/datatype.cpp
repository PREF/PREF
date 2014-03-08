#include "datatype.h"

namespace PrefSDK
{
    DataType::DataType(lua_State *l): LuaCTable(l, "DataType")
    {
        this->exportConstant("Invalid", DataType::Invalid);
        this->exportConstant("UInt8", DataType::UInt8);
        this->exportConstant("UInt16", DataType::UInt16);
        this->exportConstant("UInt32", DataType::UInt32);
        this->exportConstant("UInt64", DataType::UInt64);
        this->exportConstant("Int8", DataType::Int8);
        this->exportConstant("Int16", DataType::Int16);
        this->exportConstant("Int32", DataType::Int32);
        this->exportConstant("Int64", DataType::Int64);
        this->exportConstant("Char", DataType::Char);
        this->exportConstant("AsciiChar", DataType::AsciiChar);
        this->exportConstant("UnicodeChar", DataType::UnicodeChar);
        this->exportConstant("Blob", DataType::Blob);
        this->exportConstant("List", DataType::List);
        this->exportConstant("AsciiString", DataType::AsciiString);
        this->exportConstant("UnicodeString", DataType::UnicodeString);

        this->exportFunction<lua_Integer, lua_Integer>("sizeOf", &DataType::sizeOf);
        this->exportFunction<bool, lua_Integer>("isInteger", &DataType::isInteger);
        this->exportFunction<bool, lua_Integer>("isSigned", &DataType::isSigned);
        this->exportFunction<bool, lua_Integer>("isString", &DataType::isString);
        this->exportFunction<bool, lua_Integer>("isAscii", &DataType::isAscii);
        this->exportFunction<bool, lua_Integer>("isUnicode", &DataType::isUnicode);
        this->exportFunction<bool, lua_Integer>("isList", &DataType::isList);
    }

    DataType::Ptr DataType::create(lua_State *l)
    {
        return DataType::Ptr(new DataType(l));
    }

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

    bool DataType::isList(DataType::Type type)
    {
        return (type & DataType::Vector) != 0;
    }

    lua_Integer DataType::sizeOf(lua_Integer type)
    {
        DataType::Type dt = static_cast<DataType::Type>(type);

        if(dt & DataType::Vector)
            return 0;

        if((dt & DataType::Ascii) || (dt == DataType::UInt8) || (dt == DataType::Int8) || (dt == DataType::Blob)) /* Blob requires 1 byte (used as array element only) */
            return 1;

        if((dt & DataType::Unicode) || (dt == DataType::UInt16) || (dt == DataType::Int16))
            return 2;

        if((dt == DataType::UInt32) || (dt == DataType::Int32))
            return 4;

        if((dt == DataType::UInt64) || (dt == DataType::Int64))
            return 8;

        return 0;
    }

    bool DataType::isInteger(lua_Integer type)
    {
        return DataType::isInteger(static_cast<DataType::Type>(type));
    }

    bool DataType::isSigned(lua_Integer type)
    {
        return DataType::isSigned(static_cast<DataType::Type>(type));
    }

    bool DataType::isString(lua_Integer type)
    {
        return DataType::isString(static_cast<DataType::Type>(type));
    }

    bool DataType::isAscii(lua_Integer type)
    {
        return DataType::isAscii(static_cast<DataType::Type>(type));
    }

    bool DataType::isUnicode(lua_Integer type)
    {
        return DataType::isUnicode(static_cast<DataType::Type>(type));
    }

    bool DataType::isList(lua_Integer type)
    {
        return DataType::isList(static_cast<DataType::Type>(type));
    }

    QString DataType::stringValue(DataType::Type type)
    {
        switch(type)
        {
            case DataType::AsciiChar:
                return "AsciiChar";

            case DataType::UnicodeChar:
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

            case DataType::List:
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

    int DataType::byteLength(DataType::Type type)
    {
        switch(type)
        {
            case DataType::Blob: /* Blob requires 1 byte (used in array only) */
            case DataType::AsciiChar:
            case DataType::UInt8:
            case DataType::Int8:
                return 1;

            case DataType::UnicodeChar:
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
