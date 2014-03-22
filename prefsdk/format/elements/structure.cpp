#include "structure.h"

namespace PrefSDK
{
    Structure::Structure(const LuaTable::Ptr &s, QObject *parent): FormatElement(s, parent)
    {

    }

    lua_Integer Structure::fieldCount()
    {
        return this->_elementtable->call<lua_Integer>("fieldCount");
    }

    FormatElement *Structure::field(lua_Integer i)
    {
        FormatElement* fe = nullptr;
        LuaTable::Ptr t = this->_elementtable->call<LuaTable::Ptr, lua_Integer>("field", i + 1);

        /*
         * !!! VERY IMPORTANT !!!
         * Qt's QModelIndex compares user data too, so, we need to
         * store the pointer and update its internal table in order to
         * return the same Structure* instance for the same index.
         *
         * If the user data is different, Qt threat the columns as
         * different objects and the item's selection is wrong.
         */

        if(this->_fieldpool.contains(i))
        {
            fe = this->_fieldpool[i];
            fe->updateTable(t);
        }
        else
        {
            fe = new FormatElement(t);
            this->_fieldpool[i] = fe;
        }

        return fe;
    }
}
