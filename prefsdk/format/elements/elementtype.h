#ifndef PREFSDK_ELEMENTTYPE_H
#define PREFSDK_ELEMENTTYPE_H

#include "prefsdk/qlua.h"

namespace PrefSDK
{
    class ElementType
    {
        private:
            ElementType();

        public:
            static void load(lua_State* l);
            static lua_Integer invalid();
            static lua_Integer structure();
            static lua_Integer field();
            static lua_Integer fieldArray();
            static lua_Integer bitField();

        private:
            static LuaTable::Ptr _elementtypetable;
    };
}

#endif // PREFSDK_ELEMENTTYPE_H
