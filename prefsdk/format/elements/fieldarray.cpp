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
        Field* item = nullptr;
        LuaTable::Ptr t = this->_elementtable->call<LuaTable::Ptr, lua_Integer>("item", i + 1);

        /*
         * !!! VERY IMPORTANT !!!
         * Qt's QModelIndex compares user data too, so, we need to
         * store the pointer and update its internal table in order to
         * return the same Structure* instance for the same index.
         *
         * If the user data is different, Qt threat the columns as
         * different objects and the item's selection is wrong.
         */

        if(this->_itempool.contains(i))
        {
            item = this->_itempool[i];
            item->updateTable(t);
        }
        else
        {
            item = new Field(t);
            this->_itempool[i] = item;
        }

        return item;
    }
}
