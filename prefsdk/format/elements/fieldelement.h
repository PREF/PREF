#ifndef PREFSDK_FIELDELEMENT_H
#define PREFSDK_FIELDELEMENT_H

#include <QtCore>
#include "formatelement.h"

namespace PrefSDK
{
    using namespace Lua;

    class FieldElement : public FormatElement
    {
        Q_OBJECT

        public:
            explicit FieldElement(const LuaTable::Ptr& fo, QObject *parent = 0);
            lua_Integer dataType();
            bool isSigned();
            bool isInteger();
            bool isOverflowed();
    };
}
#endif // PREFSDK_FIELDELEMENT_H
