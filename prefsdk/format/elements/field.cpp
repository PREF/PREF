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
        BitField* bf = nullptr;
        LuaTable::Ptr t = this->_elementtable->call<LuaTable::Ptr, lua_Integer>("bitField", i + 1);

        /*
         * !!! VERY IMPORTANT !!!
         * Qt's QModelIndex compares user data too, so, we need to
         * store the pointer and update its internal table in order to
         * return the same Structure* instance for the same index.
         *
         * If the user data is different, Qt threat the columns as
         * different objects and the item's selection is wrong.
         */

        if(this->_bitfieldpool.contains(i))
        {
            bf = this->_bitfieldpool[i];
            bf->updateTable(t);
        }
        else
        {
            bf = new BitField(t);
            this->_bitfieldpool[i] = bf;
        }

        return bf;
    }
}
