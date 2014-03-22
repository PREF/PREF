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

    lua_Integer FormatTree::indexOf(FormatElement* fe)
    {
        return this->_formattreetable->call<lua_Integer, LuaTable::Ptr>("indexOf", fe->table()) - 1;
    }

    Structure *FormatTree::structure(lua_Integer i)
    {
        Structure* s = nullptr;
        LuaTable::Ptr t = this->_formattreetable->call<LuaTable::Ptr, lua_Integer>("structure", i + 1);

        /*
         * !!! VERY IMPORTANT !!!
         * Qt's QModelIndex compares user data too, so, we need to
         * store the pointer and update its internal table in order to
         * return the same Structure* instance for the same index.
         *
         * If the user data is different, Qt threat the columns as
         * different objects and the item's selection is wrong.
         */

        if(this->_structurepool.contains(i))
        {
            s = this->_structurepool[i];
            s->updateTable(t);
        }
        else
        {
            s = new Structure(t);
            this->_structurepool[i] = s;
        }

        return s;
    }

    const LuaTable::Ptr &FormatTree::luaTable()
    {
        return this->_formattreetable;
    }
}
