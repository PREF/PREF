#include "field.h"

namespace PrefSDK
{
    Field::Field(const LuaTable::Ptr &f, QObject *parent): FieldElement(f, parent)
    {

    }

    lua_Integer Field::bitFieldCount()
    {
        return this->_elementtable->call<lua_Integer>("bitFieldCount");
    }

    BitField *Field::bitField(lua_Integer i)
    {
        LuaTable::Ptr bf = this->_elementtable->call<LuaTable::Ptr, lua_Integer>("bitfield", i + 1);
        return new BitField(bf, this);
    }
}
