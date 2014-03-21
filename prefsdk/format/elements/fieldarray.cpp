#include "fieldarray.h"

namespace PrefSDK
{
    FieldArray::FieldArray(const LuaTable::Ptr &fa, QObject *parent): FieldElement(fa, parent)
    {

    }

    lua_Integer FieldArray::elementType()
    {
        return this->_elementtable->call<lua_Integer>("elementType");
    }

    lua_Integer FieldArray::elementCount()
    {
        return this->_elementtable->call<lua_Integer>("elementCount");
    }

    Field *FieldArray::element(lua_Integer i)
    {
        LuaTable::Ptr f = this->_elementtable->call<LuaTable::Ptr, lua_Integer>("element", i + 1);
        return new Field(f, this);
    }
}
