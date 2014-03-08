#ifndef FIELDOBJECT_H
#define FIELDOBJECT_H

#include <QtCore>
#include <QtWidgets>
#include "prefsdk/format/elements/formatobject.h"

namespace PrefSDK
{
    using namespace Lua;

    class FieldObject : public FormatObject
    {
        Q_OBJECT

        public:
            explicit FieldObject(lua_State *l, lua_String tablename, DataType::Type type, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable* model, FormatObject* parentobject, QObject *parent = 0);
            DataType::Type dataType();
            bool isSigned() const;
            bool isInteger() const;
            bool isOverflowed();

        public: /* Overriden methods */
            virtual QString typeName();
            virtual lua_Integer value();
            virtual QString displayValue();
            virtual lua_Integer size();

        lua_private:
            lua_Integer luaDataType();

        private:
            DataType::Type _type;
    };
}
#endif // FIELDOBJECT_H
