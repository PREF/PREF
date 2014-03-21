#include "fieldelement.h"

namespace PrefSDK
{
    FieldElement::FieldElement(lua_State* l, lua_String tablename, DataType::Type type, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable* model, FormatElement *parentobject, QObject *parent): FormatElement(l, tablename, offset, name, bytebuffer, model, parentobject, parent)
    {
        this->_type = type;

        this->exportMethod<lua_Integer, FieldElement>("datatype", &FieldElement::luaDataType);
    }

    QString FieldElement::displayType()
    {
        return DataType::stringValue(this->_type);
    }

    lua_Integer FieldElement::size()
    {
        return DataType::sizeOf(this->_type);
    }

    DataType::Type FieldElement::dataType()
    {
        return this->_type;
    }

    lua_Integer FieldElement::luaDataType()
    {
        return static_cast<lua_Integer>(this->_type);
    }

    bool FieldElement::isSigned() const
    {
        return DataType::isSigned(this->_type);
    }

    bool FieldElement::isInteger() const
    {
        return DataType::isInteger(this->_type);
    }

    bool FieldElement::isOverflowed()
    {
        return this->_bytebuffer->willOverflow(this->offset(), this->_type);
    }

    lua_Integer FieldElement::value()
    {
        return this->_bytebuffer->readType(this->offset(), this->_type);
    }

    QString FieldElement::displayValue()
    {
        if(this->_type == DataType::Char)
            return this->_bytebuffer->readString(this->offset(), 1);

        return this->_bytebuffer->stringValue(this->offset(), this->base(), this->_type);
    }
}
