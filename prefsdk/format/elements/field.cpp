#include "field.h"

namespace PrefSDK
{
    Field::Field(lua_State *l, DataType::Type datatype, uint64_t offset, const QString &name, const QUuid& parentid, ElementPool &elementpool, QHexEditData* hexeditdata, QObject *parent): FieldElement(l, datatype, offset, name, parentid, elementpool, hexeditdata, parent)
    {

    }

    uint Field::bitFieldCount() const
    {
        return this->_bitfieldids.count();
    }

    const BitField *Field::bitField(int64_t i) const
    {
        QList<QString> keys = this->_bitfieldids.keys();
        return this->bitField(keys[i]);
    }

    const BitField *Field::bitField(const QString& name) const
    {
        const QUuid& id = this->_bitfieldids[name];
        return qobject_cast<BitField*>(this->_elementpool[id]);
    }

    bool Field::isDynamic() const
    {
        if(this->_bitfieldids.isEmpty())
            return FormatElement::isDynamic();

        return false;
    }

    ElementType::Type Field::elementType() const
    {
        return ElementType::Field;
    }

    int Field::indexOf(FormatElement *fe) const
    {
        if(this->_bitfieldids.contains(fe->name()))
        {
            QList<QString> names = this->_bitfieldids.keys();

            for(int i = 0; i < names.length(); i++)
            {
                if(!QString::compare(names[i], fe->name()))
                    return i;
            }
        }

        return -1;
    }

    QString Field::displayValue() const
    {
        if(DataType::isInteger(this->_datatype))
        {
            if(this->isOverflowed())
                return "Overflow";

            int width = DataType::byteWidth(this->_datatype);

            if(DataType::isSigned(this->_datatype))
            {
                switch(DataType::bitWidth(this->_datatype))
                {
                    case 8:
                        return QString("%1").arg(static_cast<char>(this->_hexeditdata->at(this->offset())), width, this->base(), QLatin1Char('0')).toUpper();

                    case 16:
                        return QString("%1").arg(this->_hexeditdata->readInt16(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    case 32:
                        return QString("%1").arg(this->_hexeditdata->readInt32(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    case 64:
                        return QString("%1").arg(this->_hexeditdata->readInt64(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    default:
                        break;
                }
            }
            else
            {
                switch(DataType::bitWidth(this->_datatype))
                {
                    case 8:
                        return QString("%1").arg(this->_hexeditdata->at(this->offset()), width, this->base(), QLatin1Char('0')).toUpper();

                    case 16:
                        return QString("%1").arg(this->_hexeditdata->readUInt16(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    case 32:
                        return QString("%1").arg(this->_hexeditdata->readUInt32(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    case 64:
                        return QString("%1").arg(this->_hexeditdata->readUInt64(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    default:
                        break;
                }
            }
        }
        else if(this->_datatype == DataType::Character)
            return QString("'%1'").arg(QString(QByteArray().append(this->_hexeditdata->at(this->offset()))));

        return QString();
    }

    bool Field::isOverflowed() const
    {
        return NumericLimits::willOverflow(this->_hexeditdata, this->offset(), this->dataType());
    }
}
