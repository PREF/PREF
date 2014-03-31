#include "datatype.h"

namespace PrefSDK
{
    LuaTable::Ptr DataType::_datatypetable;

    DataType::DataType()
    {

    }

    void DataType::load(lua_State *l)
    {
        DataType::_datatypetable = LuaTable::global(l, "DataType");
    }

    lua_Integer DataType::invalid()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("Invalid");
    }

    lua_Integer DataType::uint8()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("UInt8");
    }

    lua_Integer DataType::uint16()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("UInt16");
    }

    lua_Integer DataType::uint32()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("UInt32");
    }

    lua_Integer DataType::uint64()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("UInt64");
    }

    lua_Integer DataType::int8()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("Int8");
    }

    lua_Integer DataType::int16()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("Int16");
    }

    lua_Integer DataType::int32()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("Int32");
    }

    lua_Integer DataType::int64()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("Int64");
    }

    lua_Integer DataType::array()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("Array");
    }

    lua_Integer DataType::blob()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("Blob");
    }

    lua_Integer DataType::character()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("Character");
    }

    lua_Integer DataType::asciiCharacter()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("AsciiCharacter");
    }

    lua_Integer DataType::unicodeCharacter()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("UnicodeCharacter");
    }

    lua_Integer DataType::asciiString()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("AsciiString");
    }

    lua_Integer DataType::unicodeString()
    {
        return DataType::_datatypetable->get<lua_String, lua_Integer>("UnicodeString");
    }

    bool DataType::isInteger(lua_Integer datatype)
    {
        return DataType::_datatypetable->call<bool, lua_Integer>("isInteger", datatype);
    }

    bool DataType::isSigned(lua_Integer datatype)
    {
        return DataType::_datatypetable->call<bool, lua_Integer>("isSigned", datatype);
    }

    bool DataType::isString(lua_Integer datatype)
    {
        return DataType::_datatypetable->call<bool, lua_Integer>("isString", datatype);
    }

    bool DataType::isAscii(lua_Integer datatype)
    {
        return DataType::_datatypetable->call<bool, lua_Integer>("isAscii", datatype);
    }

    bool DataType::isUnicode(lua_Integer datatype)
    {
        return DataType::_datatypetable->call<bool, lua_Integer>("isUnicode", datatype);
    }

    bool DataType::isArray(lua_Integer datatype)
    {
        return DataType::_datatypetable->call<bool, lua_Integer>("isArray", datatype);
    }

    lua_Integer DataType::sizeOf(lua_Integer datatype)
    {
        return DataType::_datatypetable->call<lua_Integer, lua_Integer>("sizeOf", datatype);
    }

    QString DataType::stringValue(lua_Integer datatype)
    {
        return DataType::_datatypetable->call<QString, lua_Integer>("stringValue", datatype);
    }

    lua_Integer DataType::byteWidth(lua_Integer datatype)
    {
        return DataType::_datatypetable->call<lua_Integer, lua_Integer>("byteWidth", datatype);
    }
}
