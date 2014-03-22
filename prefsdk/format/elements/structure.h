#ifndef PREFSDK_STRUCTURE_H
#define PREFSDK_STRUCTURE_H

#include <QtCore>
#include "field.h"
#include "fieldarray.h"

namespace PrefSDK
{
    class Structure: public FormatElement
    {
        Q_OBJECT

        public:
            explicit Structure(const LuaTable::Ptr& s, QObject *parent = 0);
            lua_Integer fieldCount();
            FormatElement *field(lua_Integer i);
            QString fieldId(lua_Integer i);
    };
}

#endif // PREFSDK_STRUCTURE_H
