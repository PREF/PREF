#include "unicodestring.h"

namespace PrefSDK
{
    UnicodeString::UnicodeString(lua_State *l, lua_Integer offset, QString name, quint64 itemcount, ByteBuffer *bytebuffer, LuaCTable *model, FormatObject *formatobject, QObject *parent): StringField(l, DataType::UnicodeChar, offset, name, itemcount, bytebuffer, model, formatobject, DataType::UnicodeString, parent)
    {

    }

    QString UnicodeString::displayType()
    {
        return DataType::stringValue(DataType::UnicodeString);
    }

    lua_Integer UnicodeString::size()
    {
        return this->elementCount();
    }
}
