#include "bitfield.h"

namespace PrefSDK
{
    BitField::BitField(lua_State *l, int bitstart, int bitend, uint64_t offset, DataType::Type datatype, const QString &name, const QUuid& parentid, ElementPool& elementpool, QHexEditData* hexeditdata, QObject *parent): FieldElement(l, datatype, offset, name, parentid, elementpool, hexeditdata, parent), _bitstart(bitstart), _bitend(bitend)
    {
        this->_mask = BitField::createMask(bitstart, bitend);
    }

    uint64_t BitField::mask() const
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

    ElementType::Type BitField::elementType() const
    {
        return ElementType::BitField;
    }

    uint64_t BitField::size() const
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
        quint64 val = this->_hexeditdata->readUInt64(this->offset(), DataType::byteOrder(this->dataType()));
        return QString("%1").arg((val & this->_mask) >> this->_bitstart, 0, this->base()).toUpper();
    }

    bool BitField::isOverflowed() const
    {
        return false;
    }

    uint64_t BitField::createMask(int bitstart, int bitend)
    {
        uint64_t mask = 0u;

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
