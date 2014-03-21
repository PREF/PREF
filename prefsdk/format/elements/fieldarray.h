#ifndef FIELDARRAY_H
#define FIELDARRAY_H

#include <QtCore>
#include "field.h"

namespace PrefSDK
{
    class FieldArray : public FieldElement
    {
        Q_OBJECT

        public:
            explicit FieldArray(const LuaTable::Ptr& fa, QObject *parent = 0);
            lua_Integer elementType();
            lua_Integer elementCount();
            Field* element(lua_Integer i);
    };
}
#endif // FIELDARRAY_H
