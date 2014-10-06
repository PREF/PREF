#include "fieldelement.h"

namespace PrefSDK
{
    FieldElement::FieldElement(DataType::Type datatype, quint64 offset, const QString& name, const QUuid& parentid, AbstractTree *formattree, QObject *parent): FormatElement(offset, name, parentid, formattree, parent), _datatype(datatype)
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

    quint64 FieldElement::size() const
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
