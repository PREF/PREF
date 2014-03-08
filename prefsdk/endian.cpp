#include "endian.h"

namespace PrefSDK
{
    Endian::Endian(lua_State *l): LuaCTable(l, "Endian")
    {
        this->exportConstant("PlatformEndian", QSysInfo::ByteOrder);
        this->exportConstant("LittleEndian", QSysInfo::LittleEndian);
        this->exportConstant("BigEndian", QSysInfo::BigEndian);
    }

    Endian::Ptr Endian::create(lua_State *l)
    {
        return Endian::Ptr(new Endian(l));
    }
}
