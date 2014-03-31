#include "preftable.h"

namespace PrefSDK
{
    PrefTable::PrefTable(lua_State* l): LuaCTable(l, "pref")
    {
        this->_numericlimitstable = NumericLimits::create(l);


    }

    PrefTable::Ptr PrefTable::create(lua_State *l)
    {
        return PrefTable::Ptr(new PrefTable(l));
    }
}
