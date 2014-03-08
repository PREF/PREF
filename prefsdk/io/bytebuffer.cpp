#include "bytebuffer.h"

namespace PrefSDK
{
    ByteBuffer::ByteBuffer(lua_State* l, QHexEditData *hexeditdata, lua_Integer baseoffset): AbstractBuffer(l, "ByteBuffer"), _hexeditdata(hexeditdata), _endian(QSysInfo::ByteOrder)
    {
        this->setBaseOffset(baseoffset);

        this->exportMethod<lua_Integer, ByteBuffer, lua_Integer, lua_Integer>("readType", &ByteBuffer::readType);
        this->exportMethod<void, ByteBuffer, lua_Integer, lua_Integer, lua_Integer>("writeType", &ByteBuffer::writeType);
    }

    ByteBuffer::~ByteBuffer()
    {

    }

    ByteBuffer *ByteBuffer::clone(lua_Integer baseoffset)
    {
        return new ByteBuffer(this->state(), this->_hexeditdata, (baseoffset == -1 ? this->baseOffset() : baseoffset));
    }

    QHexEditData *ByteBuffer::hexEditData()
    {
        return this->_hexeditdata;
    }

    uchar ByteBuffer::at(qint64 i)
    {
        return this->_hexeditdata->at(i);
    }

    QString ByteBuffer::stringValue(lua_Integer pos, int base, DataType::Type type)
    {
        return this->stringValue(pos, base, type, this->_endian);
    }

    QSysInfo::Endian ByteBuffer::endian()
    {
        return this->_endian;
    }

    void ByteBuffer::save()
    {
        this->_hexeditdata->save();
    }

    void ByteBuffer::insert(qint64 pos, const QByteArray &ba)
    {
        this->_hexeditdata->insert(pos, ba);
    }

    void ByteBuffer::write(qint64 pos, qint64 len, const QByteArray &ba)
    {
        this->_hexeditdata->replace(pos, len, ba);
    }

    void ByteBuffer::remove(qint64 pos, qint64 len)
    {
        this->_hexeditdata->remove(pos, len);
    }

    void ByteBuffer::setEndian(QSysInfo::Endian endian)
    {
        this->_endian = endian;
    }

    void ByteBuffer::save(QString filename)
    {
        QFile f(filename);
        this->_hexeditdata->saveTo(&f);
        f.close();
    }

    QIODevice *ByteBuffer::device()
    {
        return this->_hexeditdata->device();
    }

    QByteArray ByteBuffer::read(qint64 pos, qint64 len)
    {
        return this->_hexeditdata->read(pos, len);
    }

    bool ByteBuffer::willOverflow(lua_Integer pos, DataType::Type type) const
    {
        return this->willOverflow(pos, type, this->_endian);
    }

    bool ByteBuffer::willOverflow(lua_Integer pos, DataType::Type type, QSysInfo::Endian endian) const
    {
        switch(type)
        {
            case DataType::UInt8:
                return this->willOverflowT<DataType::UInt8>(this->readT<DataType::UInt8>(pos, endian));

            case DataType::Int8:
                return this->willOverflowT<DataType::Int8>(this->readT<DataType::Int8>(pos, endian));

            case DataType::UInt16:
                return this->willOverflowT<DataType::UInt16>(this->readT<DataType::UInt16>(pos, endian));

            case DataType::Int16:
                return this->willOverflowT<DataType::Int16>(this->readT<DataType::Int16>(pos, endian));

            case DataType::UInt32:
                return this->willOverflowT<DataType::UInt32>(this->readT<DataType::UInt32>(pos, endian));

            case DataType::Int32:
                return this->willOverflowT<DataType::Int32>(this->readT<DataType::Int32>(pos, endian));

            case DataType::UInt64:
                return this->willOverflowT<DataType::UInt64>(this->readT<DataType::UInt64>(pos, endian));

            case DataType::Int64:
                return this->willOverflowT<DataType::Int64>(this->readT<DataType::Int64>(pos, endian));

            default:
                break;
        }

        return false;
    }

    QString ByteBuffer::stringValue(lua_Integer pos, int base, DataType::Type type, QSysInfo::Endian endian)
    {
        switch(type)
        {
            case DataType::UInt8:
            {
                Data::ToType<DataType::UInt8>::Type v = this->readT<DataType::UInt8>(pos, endian);

                if(this->willOverflowT<DataType::UInt8>(v))
                    return "Overflow";

                return QString("%1").arg(v, 2, base, QLatin1Char('0')).toUpper();
            }

            case DataType::Int8:
            {
                Data::ToType<DataType::Int8>::Type v = this->readT<DataType::Int8>(pos, endian);

                if(this->willOverflowT<DataType::Int8>(v))
                    return "Overflow";

                return QString("%1").arg(v, 2, base, QLatin1Char('0')).toUpper();
            }

            case DataType::UInt16:
            {
                Data::ToType<DataType::UInt16>::Type v = this->readT<DataType::UInt16>(pos, endian);

                if(this->willOverflowT<DataType::UInt16>(v))
                    return "Overflow";

                return QString("%1").arg(v, 4, base, QLatin1Char('0')).toUpper();
            }

            case DataType::Int16:
            {
                Data::ToType<DataType::Int16>::Type v = this->readT<DataType::Int16>(pos, endian);

                if(this->willOverflowT<DataType::Int16>(v))
                    return "Overflow";

                return QString("%1").arg(v, 4, base, QLatin1Char('0')).toUpper();
            }

            case DataType::UInt32:
            {
                Data::ToType<DataType::UInt32>::Type v = this->readT<DataType::UInt32>(pos, endian);

                if(this->willOverflowT<DataType::UInt32>(v))
                    return "Overflow";

                return QString("%1").arg(v, 8, base, QLatin1Char('0')).toUpper();
            }

            case DataType::Int32:
            {
                Data::ToType<DataType::Int32>::Type v = this->readT<DataType::Int32>(pos, endian);

                if(this->willOverflowT<DataType::Int32>(v))
                    return "Overflow";

                return QString("%1").arg(v, 8, base, QLatin1Char('0')).toUpper();
            }

            case DataType::UInt64:
            {
                Data::ToType<DataType::UInt64>::Type v = this->readT<DataType::UInt64>(pos, endian);

                if(this->willOverflowT<DataType::UInt64>(v))
                    return "Overflow";

                return QString("%1").arg(v, 16, base, QLatin1Char('0')).toUpper();
            }

            case DataType::Int64:
            {
                Data::ToType<DataType::Int64>::Type v = this->readT<DataType::Int64>(pos, endian);

                if(this->willOverflowT<DataType::Int64>(v))
                    return "Overflow";

                return QString("%1").arg(v, 16, base, QLatin1Char('0')).toUpper();
            }

            default:
                break;
        }

        return QString();
    }

    PrefSDK::ByteBuffer::operator QHexEditData *() const
    {
        return this->_hexeditdata;
    }

    lua_Integer ByteBuffer::readType(lua_Integer pos, lua_Integer type)
    {
        switch(type)
        {
            case DataType::UInt8:
                return static_cast<lua_Integer>(this->readT<DataType::UInt8>(pos, this->_endian));

            case DataType::Int8:
                return static_cast<lua_Integer>(this->readT<DataType::Int8>(pos, this->_endian));

            case DataType::UInt16:
                return static_cast<lua_Integer>(this->readT<DataType::UInt16>(pos, this->_endian));

            case DataType::Int16:
                return static_cast<lua_Integer>(this->readT<DataType::Int16>(pos, this->_endian));

            case DataType::UInt32:
                return static_cast<lua_Integer>(this->readT<DataType::UInt32>(pos, this->_endian));

            case DataType::Int32:
                return static_cast<lua_Integer>(this->readT<DataType::Int32>(pos, this->_endian));

            case DataType::UInt64:
                return static_cast<lua_Integer>(this->readT<DataType::UInt64>(pos, this->_endian));

            case DataType::Int64:
                return static_cast<lua_Integer>(this->readT<DataType::Int64>(pos, this->_endian));

            default:
                break;
        }

        throw LuaException("ByteBuffer::read(): Unsupported Type");
    }

    lua_Integer ByteBuffer::length() const
    {
        return static_cast<lua_Integer>(this->_hexeditdata->length()) - this->baseOffset();
    }

    void ByteBuffer::writeType(lua_Integer pos, lua_Integer datatype, lua_Integer val)
    {
        this->adjustOffset(pos);

        DataType::Type dt = static_cast<DataType::Type>(datatype);
        this->writeInteger(pos, dt, val);
    }

    void ByteBuffer::writeInteger(lua_Integer pos, DataType::Type type, lua_Integer val)
    {
        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly);

        if(this->_endian == QSysInfo::LittleEndian)
            ds.setByteOrder(QDataStream::LittleEndian);
        else /* if(endian == QSysInfo::BigEndian) */
            ds.setByteOrder(QDataStream::BigEndian);

        switch(type)
        {
            case DataType::UInt8:
                ds << static_cast<quint8>(val);
                break;

            case DataType::Int8:
                ds << static_cast<qint8>(val);
                break;

            case DataType::UInt16:
                ds << static_cast<quint16>(val);
                break;

            case DataType::Int16:
                ds << static_cast<qint16>(val);
                break;

            case DataType::UInt32:
                ds << static_cast<quint32>(val);
                break;

            case DataType::Int32:
                ds << static_cast<qint32>(val);
                break;

            case DataType::UInt64:
                ds << static_cast<quint64>(val);
                break;

            case DataType::Int64:
                ds << static_cast<qint64>(val);
                break;

            default:
                break;
        }

        if(!ba.isEmpty())
            this->_hexeditdata->replace(pos, ba.length(), ba);
    }
}
