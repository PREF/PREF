#include "stringfield.h"

namespace PrefSDK
{
    StringField::StringField(lua_State* l, DataType::Type itemtype, lua_Integer offset, QString name, quint64 itemcount, ByteBuffer* bytebuffer, LuaCTable* model, FormatElement* formatobject, DataType::Type type, QObject *parent): FieldArray(l, itemtype, offset, name, itemcount, bytebuffer, model, formatobject, type, parent)
    {
    }

    QString StringField::displayName()
    {
        return FormatElement::displayName();
    }

    QString StringField::displayValue()
    {
        qint64 len = this->elementCount();
        QString s = this->_bytebuffer->readString(this->offset(), this->elementCount());

        if(!s.at(s.length() - 1).isPrint()) /* Strip Null Terminator, if any */
            len--;

        return QString("'%1'").arg(this->_bytebuffer->readString(this->offset(), len));
    }
}
