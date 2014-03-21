#ifndef PREFSDK_FIELDELEMENT_H
#define PREFSDK_FIELDELEMENT_H

#include <QtCore>
#include <QtWidgets>
#include "prefsdk/format/elements/formatelement.h"

namespace PrefSDK
{
    using namespace Lua;

    class FieldElement : public FormatElement
    {
        Q_OBJECT

        public:
            explicit FieldElement(lua_State *l, lua_String tablename, DataType::Type type, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable* model, FormatElement* parentobject, QObject *parent = 0);
            DataType::Type dataType();
            bool isSigned() const;
            bool isInteger() const;
            bool isOverflowed();

        public: /* Overriden methods */
            virtual QString displayType();
            virtual lua_Integer value();
            virtual QString displayValue();
            virtual lua_Integer size();

        lua_private:
            lua_Integer luaDataType();

        private:
            DataType::Type _type;
    };
}
#endif // PREFSDK_FIELDELEMENT_H
