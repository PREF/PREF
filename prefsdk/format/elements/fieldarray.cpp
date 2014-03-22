#include "fieldarray.h"

namespace PrefSDK
{
    FieldArray::FieldArray(const LuaTable::Ptr &fa, QObject *parent): FieldElement(fa, parent)
    {

    }

    lua_Integer FieldArray::itemType()
    {
        return this->_elementtable->call<lua_Integer>("itemType");
    }

    lua_Integer FieldArray::itemCount()
    {
        return this->_elementtable->call<lua_Integer>("itemCount");
    }

    Field *FieldArray::item(lua_Integer i)
    {
        LuaTable::Ptr t = this->_elementtable->call<LuaTable::Ptr, lua_Integer>("item", i + 1);
        return new Field(t);
    }

    QString FieldArray::itemId(lua_Integer i)
    {
        return this->_elementtable->call<QString>("itemId", i + 1);
    }
}
