#include "fieldelement.h"

namespace PrefSDK
{
    FieldElement::FieldElement(lua_State *l, DataType::Type datatype, uint64_t offset, const QString& name, const QUuid& parentid, ElementPool &elementpool, QHexEditData* hexeditdata, QObject *parent): FormatElement(l, offset, name, parentid, elementpool, hexeditdata, parent), _datatype(datatype)
    {

    }

    DataType::Type FieldElement::dataType() const
    {
        return this->_datatype;
    }

    bool FieldElement::isSigned() const
    {
        return DataType::isSigned(this->_datatype);
    }

    bool FieldElement::isInteger() const
    {
        return DataType::isInteger(this->_datatype);
    }

    uint64_t FieldElement::size() const
    {
        return DataType::sizeOf(this->_datatype);
    }

    QString FieldElement::displayType() const
    {
        return DataType::stringValue(this->_datatype);
    }

    QString FieldElement::displayValue() const
    {
        return QString();
    }

    int FieldElement::indexOf(FormatElement*) const
    {
        return -1;
    }
}
