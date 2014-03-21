#include "elementtype.h"

namespace PrefSDK
{
    LuaTable::Ptr ElementType::_elementtypetable;

    ElementType::ElementType()
    {
    }

    void ElementType::load(lua_State *l)
    {
        ElementType::_elementtypetable = LuaTable::global(l, "ElementType");
    }

    lua_Integer ElementType::invalid()
    {
        return ElementType::_elementtypetable->get<lua_String, lua_Integer>("Invalid");
    }

    lua_Integer ElementType::structure()
    {
        return ElementType::_elementtypetable->get<lua_String, lua_Integer>("Structure");
    }

    lua_Integer ElementType::field()
    {
        return ElementType::_elementtypetable->get<lua_String, lua_Integer>("Field");
    }

    lua_Integer ElementType::fieldArray()
    {
        return ElementType::_elementtypetable->get<lua_String, lua_Integer>("FieldArray");
    }

    lua_Integer ElementType::bitField()
    {
        return ElementType::_elementtypetable->get<lua_String, lua_Integer>("BitField");
    }
}
