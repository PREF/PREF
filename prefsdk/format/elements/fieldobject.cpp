#include "fieldobject.h"

namespace PrefSDK
{
    FieldObject::FieldObject(lua_State* l, lua_String tablename, DataType::Type type, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable* model, FormatObject *parentobject, QObject *parent): FormatObject(l, tablename, offset, name, bytebuffer, model, parentobject, parent)
    {
        this->_type = type;

        this->exportMethod<lua_Integer, FieldObject>("datatype", &FieldObject::luaDataType);
    }

    QString FieldObject::typeName()
    {
        return DataType::stringValue(this->_type);
    }

    lua_Integer FieldObject::size()
    {
        return DataType::sizeOf(this->_type);
    }

    DataType::Type FieldObject::dataType()
    {
        return this->_type;
    }

    lua_Integer FieldObject::luaDataType()
    {
        return static_cast<lua_Integer>(this->_type);
    }

    bool FieldObject::isSigned() const
    {
        return DataType::isSigned(this->_type);
    }

    bool FieldObject::isInteger() const
    {
        return DataType::isInteger(this->_type);
    }

    bool FieldObject::isOverflowed()
    {
        return this->_bytebuffer->willOverflow(this->offset(), this->_type);
    }

    lua_Integer FieldObject::value()
    {
        return this->_bytebuffer->readType(this->offset(), this->_type);
    }

    QString FieldObject::displayValue()
    {
        if(this->_type == DataType::Char)
            return this->_bytebuffer->readString(this->offset(), 1);

        return this->_bytebuffer->stringValue(this->offset(), this->base(), this->_type);
    }
}
