#include "bitfield.h"

namespace PrefSDK
{
    BitField::BitField(int bitstart, int bitend, quint64 offset, DataType::Type datatype, const QString &name, const QUuid& parentid, AbstractTree *formattree, QObject *parent): FieldElement(datatype, offset, name, parentid, formattree, parent), _bitstart(bitstart), _bitend(bitend)
    {
        this->_mask = BitField::createMask(bitstart, bitend);
    }

    quint64 BitField::mask() const
    {
        return this->_mask;
    }

    int BitField::bitStart() const
    {
        return this->_bitstart;
    }

    int BitField::bitEnd() const
    {
        return this->_bitend;
    }

    FormatElement::Type BitField::elementType() const
    {
        return FormatElement::BitFieldType;
    }

    quint64 BitField::size() const
    {
        return 1; // No Size for BitFields.
    }

    QString BitField::displayType() const
    {
        if(this->_bitstart != this->_bitend)
            return "bit[]";

        return "bit";
    }

    QString BitField::displayName() const
    {
        if(this->_bitstart == this->_bitend)
            return QString("%1[%2]").arg(this->name(), QString::number(this->_bitstart));

        return QString("%1[%2..%3]").arg(this->name(), QString::number(this->_bitstart), QString::number(this->_bitend));
    }

    QString BitField::displayValue() const
    {
        QHexEditDataReader reader(this->_formattree->data());
        quint64 val = reader.readUInt64(this->offset(), DataType::byteOrder(this->dataType()));
        return QString("%1").arg((val & this->_mask) >> this->_bitstart, 0, this->base()).toUpper();
    }

    bool BitField::isOverflowed() const
    {
        return false;
    }

    void BitField::pushValue(lua_State *l)
    {
        QHexEditDataReader reader(this->_formattree->data());
        FieldElement* parentelement = qobject_cast<FieldElement*>(this->parentElement());

        switch(DataType::bitWidth(parentelement->dataType()))
        {
            case 8:
            {
                if(this->isSigned())
                    lua_pushinteger(l, (static_cast<qint8>(reader.at(this->offset())) & this->_mask) >> this->_bitstart);
                else
                    lua_pushinteger(l, (static_cast<quint8>(reader.at(this->offset())) & this->_mask) >> this->_bitstart);

                return;
            }

            case 16:
            {
                if(this->isSigned())
                    lua_pushinteger(l,(reader.readInt16(this->offset()) & this->_mask) >> this->_bitstart);
                else
                    lua_pushinteger(l,(reader.readUInt16(this->offset()) & this->_mask) >> this->_bitstart);

                return;
            }

            case 32:
            {
                if(this->isSigned())
                    lua_pushinteger(l,(reader.readInt32(this->offset()) & this->_mask) >> this->_bitstart);
                else
                    lua_pushinteger(l,(reader.readUInt32(this->offset()) & this->_mask) >> this->_bitstart);

                return;
            }

            case 64:
            {
                if(this->isSigned())
                    lua_pushinteger(l,(reader.readInt64(this->offset()) & this->_mask) >> this->_bitstart);
                else
                    lua_pushinteger(l,(reader.readUInt64(this->offset()) & this->_mask) >> this->_bitstart);

                return;
            }

            default:
                break;
        }

        FieldElement::pushValue(l);
    }

    quint64 BitField::createMask(int bitstart, int bitend)
    {
        quint64 mask = 0u;

        for(int i = 0; i < 32; i++)
        {
            if((i >= bitstart) && (i <= bitend))
                mask |= 1u << i;
            else
                mask |= 0u << i;
        }

        return mask;
    }
}
