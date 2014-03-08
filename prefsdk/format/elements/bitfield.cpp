#include "bitfield.h"

namespace PrefSDK
{
    BitField::BitField(lua_State* l, int bitstart, int bitend, lua_Integer offset, QString name, ByteBuffer *bytebuffer, LuaCTable* model, FieldObject *parentobject, QObject *parent): FieldObject(l, "BitField", parentobject->dataType(), offset, name, bytebuffer, model, parentobject, parent)
    {
        this->_bitstart = bitstart;
        this->_bitend = bitend;
        this->_mask = this->createMask();
    }

    QString BitField::typeName()
    {
        return QString("bit[]");
    }

    int BitField::bitStart() const
    {
        return this->_bitstart;
    }

    int BitField::bitEnd() const
    {
        return this->_bitend;
    }

    lua_Integer BitField::value()
    {
        lua_Integer val = this->parentObject()->value();
        return (val & this->_mask) >> this->_bitstart;
    }

    QString BitField::displayName()
    {
        if(this->_bitstart != this->_bitend)
            return QString("%1[%2..%3]").arg(this->name(), QString::number(this->_bitstart, this->base()).toUpper(), QString::number(this->_bitend, this->base()).toUpper());

        return QString("%1[%2]").arg(this->name(), QString::number(this->_bitstart, this->base()).toUpper());
    }

    QString BitField::displayValue()
    {
        lua_Integer val = this->value();
        return QString::number(val, this->base()).toUpper();
    }

    quint32 BitField::createMask() const
    {
        quint32 mask = 0x00000000;

        for(int i = 0; i < 32; i++)
        {
            if((i >= this->_bitstart) && (i <= this->_bitend))
                mask |= (1 << i);
            else
                mask |= (0 << i);
        }

        return mask;
    }

    FormatObject::FormatObjectType BitField::objectType()
    {
        return FormatObject::BitFieldType;
    }

    lua_Integer BitField::size()
    {
        return 0; /* No size for Bit Fields! */
    }
}
