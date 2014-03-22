#include "bitfield.h"

namespace PrefSDK
{
    BitField::BitField(const LuaTable::Ptr &bf, QObject *parent): FieldElement(bf, parent)
    {

    }

    lua_Integer BitField::bitStart()
    {
        return this->_elementtable->call<lua_Integer>("bitStart");
    }

    lua_Integer BitField::bitEnd()
    {
        return this->_elementtable->call<lua_Integer>("bitEnd");
    }
}
