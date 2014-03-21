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

    FormatElement* Structure::field(lua_Integer i)
    {
        LuaTable::Ptr f = this->_elementtable->call<LuaTable::Ptr, lua_Integer>("field", i + 1);
        return new FormatElement(f, this);
    }
}
