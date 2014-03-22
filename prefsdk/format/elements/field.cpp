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
        LuaTable::Ptr t = this->_elementtable->call<LuaTable::Ptr, lua_Integer>("bitField", i + 1);
        return new BitField(t);
    }

    QString Field::bitFieldId(lua_Integer i)
    {
        return this->_elementtable->call<QString>("bitFieldId", i + 1);
    }
}
