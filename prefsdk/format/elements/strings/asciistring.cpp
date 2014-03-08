#include "asciistring.h"

namespace PrefSDK
{
    AsciiString::AsciiString(lua_State *l, lua_Integer offset, QString name, quint64 itemcount, ByteBuffer *bytebuffer, LuaCTable *model, FormatObject *formatobject, QObject *parent): StringField(l, DataType::AsciiChar, offset, name, itemcount, bytebuffer, model, formatobject, DataType::AsciiString, parent)
    {
    }

    QString AsciiString::typeName()
    {
        return DataType::stringValue(DataType::AsciiString);
    }

    lua_Integer AsciiString::size()
    {
        return this->elementCount();
    }
}
