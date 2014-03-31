#include "byteorder.h"

namespace PrefSDK
{
    lua_Integer ByteOrder::_platformendian = -1;
    lua_Integer ByteOrder::_littleendian = -1;
    lua_Integer ByteOrder::_bigendian = -1;

    ByteOrder::ByteOrder()
    {
        //this->exportConstant("PlatformEndian", QSysInfo::ByteOrder);
        //this->exportConstant("LittleEndian", QSysInfo::LittleEndian);
        //this->exportConstant("BigEndian", QSysInfo::BigEndian);
    }

    void ByteOrder::load(lua_State *l)
    {
        LuaTable::Ptr byteordertable = LuaTable::global(l, "ByteOrder");

        ByteOrder::_platformendian = byteordertable->get<lua_String, lua_Integer>("PlatformEndian");
        ByteOrder::_littleendian = byteordertable->get<lua_String, lua_Integer>("LittleEndian");
        ByteOrder::_bigendian = byteordertable->get<lua_String, lua_Integer>("BigEndian");
    }

    QSysInfo::Endian ByteOrder::qEndian(lua_Integer sdkendian)
    {
        if(sdkendian == ByteOrder::_littleendian)
            return QSysInfo::LittleEndian;
        else if(sdkendian == ByteOrder::_bigendian)
            return QSysInfo::BigEndian;

        return QSysInfo::ByteOrder;
    }

    lua_Integer ByteOrder::sdkEndian(QSysInfo::Endian qendian)
    {
        switch(qendian)
        {
            case QSysInfo::LittleEndian:
                return ByteOrder::_littleendian;

            case QSysInfo::BigEndian:
                return ByteOrder::_bigendian;

            default:
                break;
        }

        return ByteOrder::_platformendian;
    }
}
