#include "bytebuffer.h"

namespace PrefSDK
{
    ByteBuffer::ByteBuffer(lua_State* l, QHexEditData *hexeditdata, lua_Integer baseoffset): AbstractBuffer(l, "ByteBuffer"), _hexeditdata(hexeditdata), _endian(QSysInfo::ByteOrder)
    {
        this->setBaseOffset(baseoffset);

        this->exportMethod<lua_Integer, ByteBuffer, lua_Integer, lua_Integer>("readType", &ByteBuffer::readType);
        this->exportMethod<void, ByteBuffer, lua_Integer, lua_Integer, lua_Integer>("writeType", &ByteBuffer::writeType);
        this->exportMethod<QString, ByteBuffer, lua_Integer, lua_Integer, lua_Integer>("stringValue", &ByteBuffer::stringValue);
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

    QString ByteBuffer::stringValue(lua_Integer pos, lua_Integer base, lua_Integer datatype)
    {
        return this->stringValue(pos, base, datatype, this->_endian);
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

    QString ByteBuffer::stringValue(lua_Integer pos, int base, lua_Integer datatype, QSysInfo::Endian endian)
    {
        lua_Integer val = this->readType(pos, datatype, ByteOrder::sdkEndian(endian));
        return QString("%1").arg(val, DataType::byteWidth(datatype), base, QLatin1Char('0')).toUpper();
    }

    PrefSDK::ByteBuffer::operator QHexEditData *() const
    {
        return this->_hexeditdata;
    }

    lua_Integer ByteBuffer::length() const
    {
        return static_cast<lua_Integer>(this->_hexeditdata->length()) - this->_baseoffset;
    }

    lua_Integer ByteBuffer::readType(lua_Integer pos, lua_Integer type)
    {
        return this->readType(pos, type, ByteOrder::sdkEndian(this->_endian));
    }

    lua_Integer ByteBuffer::readType(lua_Integer pos, lua_Integer datatype, lua_Integer endian)
    {
        this->adjustOffset(pos);
        QByteArray ba = this->_hexeditdata->read(pos, DataType::sizeOf(datatype));

        QDataStream ds(ba);

        if(ByteOrder::qEndian(endian) == QSysInfo::LittleEndian)
            ds.setByteOrder(QDataStream::LittleEndian);
        else
            ds.setByteOrder(QDataStream::BigEndian);

        lua_Integer t; //NOTE: NON BASTA UN LUA INTEGER!
        //ds >> t;
        return t;
    }

    void ByteBuffer::writeType(lua_Integer pos, lua_Integer datatype, lua_Integer val)
    {
        this->adjustOffset(pos);

        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly);

        if(this->_endian == QSysInfo::LittleEndian)
            ds.setByteOrder(QDataStream::LittleEndian);
        else
            ds.setByteOrder(QDataStream::BigEndian);

        if(datatype == DataType::uint8())
            ds << static_cast<quint8>(val);
        else if(datatype == DataType::uint16())
            ds << static_cast<quint16>(val);
        else if(datatype == DataType::uint16())
            ds << static_cast<quint16>(val);
        else if(datatype == DataType::uint32())
            ds << static_cast<quint32>(val);
        else if(datatype == DataType::int8())
            ds << static_cast<qint8>(val);
        else if(datatype == DataType::int16())
            ds << static_cast<qint16>(val);
        else if(datatype == DataType::int16())
            ds << static_cast<qint16>(val);
        else if(datatype == DataType::int32())
            ds << static_cast<qint32>(val);

        if(!ba.isEmpty())
            this->_hexeditdata->replace(pos, ba.length(), ba);
    }
}
