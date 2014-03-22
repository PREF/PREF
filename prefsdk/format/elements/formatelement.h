#ifndef PREFSDK_FORMATELEMENT_H
#define PREFSDK_FORMATELEMENT_H

#include <QtCore>
#include "prefsdk/qlua.h"
#include "prefsdk/io/bytebuffer.h"
#include "elementtype.h"

namespace PrefSDK
{
    using namespace Lua;

    class FormatElement : public QObject
    {
        Q_OBJECT

        public:
            explicit FormatElement(const LuaTable::Ptr& elementtable, QObject *parent = 0);
            void updateTable(const LuaTable::Ptr t);
            const LuaTable::Ptr& table();
            lua_Integer elementType();
            lua_Integer base();
            lua_Integer indexOf(FormatElement* fe);
            lua_Integer size();
            lua_Integer offset();
            lua_Integer endOffset();
            QString displayType();
            QString displayName();
            QString displayValue();
            QString info();
            FormatElement* parentElement();
            bool hasParent();
            bool containsOffset(lua_Integer offset);
            void setBase(lua_Integer b);

        signals:
            void baseChanged(FormatElement* sender);

        protected:
            LuaTable::Ptr _elementtable;
    };

    template<typename T> T* formatelement_cast(FormatElement* fe)
    {
        return new T(fe->table(), fe);
    }
}

#endif // PREFSDK_FORMATELEMENT_H
