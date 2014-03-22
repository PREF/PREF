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
            lua_Integer itemType();
            lua_Integer itemCount();
            Field* item(lua_Integer i);

        private:
            QHash<lua_Integer, Field*> _itempool;
    };
}
#endif // FIELDARRAY_H
