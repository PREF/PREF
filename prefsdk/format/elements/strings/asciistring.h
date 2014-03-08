#ifndef PREFSDK_ASCIISTRING_H
#define PREFSDK_ASCIISTRING_H

#include "stringfield.h"

namespace PrefSDK
{
    class AsciiString : public StringField
    {
        Q_OBJECT

        public:
            explicit AsciiString(lua_State* l, lua_Integer offset, QString name, quint64 itemcount, ByteBuffer* bytebuffer, LuaCTable* model, FormatObject* formatobject, QObject *parent = 0);

        public: /* Overriden methods */
            virtual QString typeName();
            virtual lua_Integer size();
    };
}

#endif // PREFSDK_ASCIISTRING_H
