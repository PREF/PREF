#include "formattree.h"

namespace PrefSDK
{
    FormatTree::FormatTree(ByteBuffer* bb, QObject* parent): QObject(parent)
    {
        LuaTable::Ptr t = LuaTable::global((*bb).state(), "FormatTree");
        this->_formattreetable = t->call<LuaTable::Ptr, LuaTable::Ptr>("new", *bb);
    }

    FormatTree::Ptr FormatTree::create(ByteBuffer* bb)
    {
        return FormatTree::Ptr(new FormatTree(bb));
    }

    lua_Integer FormatTree::structureCount()
    {
        return this->_formattreetable->call<lua_Integer>("structureCount");
    }

    lua_Integer FormatTree::indexOf(lua_Integer offset)
    {
        return this->_formattreetable->call<lua_Integer, lua_Integer>("structureCount", offset);
    }

    Structure *FormatTree::structure(lua_Integer i)
    {
        LuaTable::Ptr s = this->_formattreetable->call<LuaTable::Ptr, lua_Integer>("structure", i + 1);
        return new Structure(s, this);
    }

    const LuaTable::Ptr &FormatTree::luaTable()
    {
        return this->_formattreetable;
    }
}
