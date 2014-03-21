#ifndef PREFSDK_BITFIELD_H
#define PREFSDK_BITFIELD_H

#include <QtCore>
#include "fieldelement.h"

namespace PrefSDK
{
    class BitField : public FieldElement
    {
        public:
            BitField(const LuaTable::Ptr& bf, QObject *parent = 0);
            lua_Integer bitStart();
            lua_Integer bitEnd();
    };
}
#endif // PREFSDK_BITFIELD_H
