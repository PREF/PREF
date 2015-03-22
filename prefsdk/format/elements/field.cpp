#include "field.h"

namespace PrefSDK
{
    Field::Field(DataType::Type datatype, quint64 offset, const QString &name, const QUuid& parentid, AbstractTree *formattree, QObject *parent): FieldElement(datatype, offset, name, parentid, formattree, parent)
    {

    }

    uint Field::bitFieldCount() const
    {
        return this->_bitfieldids.count();
    }

    const BitField *Field::bitField(lua_Integer i) const
    {
        QList<QString> keys = this->_bitfieldids.keys();
        return this->bitField(keys[i]);
    }

    const BitField *Field::bitField(const QString& name) const
    {
        return qobject_cast<BitField*>(this->_formattree->elementFromPoolByUUID(this->_bitfieldids[name]));
    }

    PrefSDK::BitField *Field::setBitField(const QString &name, lua_Integer bitstart, lua_Integer bitend)
    {
        BitField* bf = new BitField(bitstart, bitend, this->offset(), this->_datatype, name, this->id(), this->_formattree, this);
        this->_bitfieldids[name] = bf->id();
        return bf;
    }

    BitField *Field::setBitField(const QString &name, lua_Integer bitstart)
    {
        return this->setBitField(name, bitstart, bitstart);
    }

    void Field::pushValue(lua_State *l)
    {
        QHexEditDataReader reader(this->_formattree->data());

        if(this->isInteger())
        {
            switch(DataType::bitWidth(this->_datatype))
            {
                case 8:
                {
                    if(this->isSigned())
                        lua_pushinteger(l, static_cast<qint8>(reader.at(this->offset())));
                    else
                        lua_pushinteger(l, static_cast<quint8>(reader.at(this->offset())));

                    return;
                }

                case 16:
                {
                    if(this->isSigned())
                        lua_pushinteger(l, reader.readInt16(this->offset(), DataType::byteOrder(this->_datatype)));
                    else
                        lua_pushinteger(l, reader.readUInt16(this->offset(), DataType::byteOrder(this->_datatype)));

                    return;
                }

                case 32:
                {
                    if(this->isSigned())
                        lua_pushinteger(l, reader.readInt32(this->offset(), DataType::byteOrder(this->_datatype)));
                    else
                        lua_pushinteger(l, reader.readUInt32(this->offset(), DataType::byteOrder(this->_datatype)));

                    return;
                }

                case 64:
                {
                    if(this->isSigned())
                        lua_pushinteger(l, reader.readInt64(this->offset(), DataType::byteOrder(this->_datatype)));
                    else
                        lua_pushinteger(l, reader.readUInt64(this->offset(), DataType::byteOrder(this->_datatype)));

                    return;
                }

                default:
                    break;
            }
        }
        else if(this->_datatype == DataType::Character)
        {
            lua_pushstring(l, QString().append(QChar(reader.at(this->offset()))).toUtf8().constData());
            return;
        }

        FieldElement::pushValue(l);
    }

    int Field::metaIndex(lua_State *l, const QString &key)
    {
        if(this->_bitfieldids.contains(key))
        {
            QtLua::pushObject(l, this->_formattree->elementFromPoolByUUID(this->_bitfieldids[key]));
            return 1;
        }

        return FieldElement::metaIndex(l, key);
    }

    bool Field::isDynamic() const
    {
        return false;
    }

    bool Field::hasChildren() const
    {
        return !this->_bitfieldids.isEmpty();
    }

    FormatElement::Type Field::elementType() const
    {
        return FormatElement::FieldType;
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
        QHexEditDataReader reader(this->_formattree->data());

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
                        return QString("%1").arg(static_cast<char>(reader.at(this->offset())), width, this->base(), QLatin1Char('0')).toUpper();

                    case 16:
                        return QString("%1").arg(reader.readInt16(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    case 32:
                        return QString("%1").arg(reader.readInt32(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    case 64:
                        return QString("%1").arg(reader.readInt64(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    default:
                        break;
                }
            }
            else
            {
                switch(DataType::bitWidth(this->_datatype))
                {
                    case 8:
                        return QString("%1").arg(reader.at(this->offset()), width, this->base(), QLatin1Char('0')).toUpper();

                    case 16:
                        return QString("%1").arg(reader.readUInt16(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    case 32:
                        return QString("%1").arg(reader.readUInt32(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    case 64:
                        return QString("%1").arg(reader.readUInt64(this->offset(), DataType::byteOrder(this->_datatype)), width, this->base(), QLatin1Char('0')).toUpper();

                    default:
                        break;
                }
            }
        }
        else if(this->_datatype == DataType::Character)
            return QString("'%1'").arg(QString(QByteArray().append(reader.at(this->offset()))));

        return QString();
    }

    bool Field::isOverflowed() const
    {
        return NumericLimits::willOverflow(this->_formattree->data(), this->offset(), this->dataType());
    }

    void Field::setBase(int b)
    {
        FieldElement::setBase(b);

        for(QHash<QString, QUuid>::Iterator it = this->_bitfieldids.begin(); it != this->_bitfieldids.end(); it++)
        {
            FormatElement* element = this->_formattree->elementFromPoolByUUID(it.value());
            element->setBase(b);
        }
    }
}
