#ifndef PREFSDK_FIELD_H
#define PREFSDK_FIELD_H

#include <QtCore>
#include "fieldelement.h"
#include "bitfield.h"

namespace PrefSDK
{
    class Field : public FieldElement
    {
        Q_OBJECT

        public:
            explicit Field(const LuaTable::Ptr& f, QObject *parent = 0);
            lua_Integer bitFieldCount();
            BitField* bitField(lua_Integer i);

        private:
            QHash<lua_Integer, BitField*> _bitfieldpool;
    };
}

#endif // PREFSDK_FIELD_H
