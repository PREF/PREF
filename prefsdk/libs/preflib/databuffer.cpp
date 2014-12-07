#include "databuffer.h"

namespace PrefSDK
{
    DataBuffer::DataBuffer(QHexEditData* hexeditdata, lua_Integer baseoffset,QObject *parent): QObject(parent), _hexeditdata(hexeditdata), _baseoffset(baseoffset)
    {
        this->_reader = new QHexEditDataReader(hexeditdata, this);
        this->_writer = new QHexEditDataWriter(hexeditdata, this);
    }

    QHexEditData *DataBuffer::hexEditData() const
    {
        return this->_hexeditdata;
    }

    lua_Integer DataBuffer::length() const
    {
        return this->_hexeditdata->length();
    }

    void DataBuffer::copyTo(QObject *databuffer, lua_Integer start, lua_Integer end)
    {
        DataBuffer* databufferout = qobject_cast<DataBuffer*>(databuffer);

        QHexEditDataDevice datain(this->_hexeditdata);
        QHexEditDataDevice dataout(databufferout->_hexeditdata);

        datain.open(QIODevice::ReadOnly);
        dataout.open(QIODevice::WriteOnly);

        datain.seek(start);
        dataout.write(datain.read(end - start));

        dataout.close();
        datain.close();

        databufferout->_hexeditdata->save();
    }

    lua_Integer DataBuffer::indexOf(const QString &s, lua_Integer startpos)
    {
        return this->_reader->indexOf(s.toUtf8(), this->_baseoffset + startpos);
    }

    QByteArray DataBuffer::read(lua_Integer pos, lua_Integer len)
    {
        return this->_reader->read(this->_baseoffset + pos, len);
    }

    lua_Integer DataBuffer::readType(lua_Integer pos, lua_Integer datatype)
    {
        DataType::Type dt = static_cast<DataType::Type>(datatype);
        pos += this->_baseoffset;

        if(DataType::isInteger(dt))
        {
            switch(DataType::bitWidth(dt))
            {
                case 8:
                {
                    if(DataType::isSigned(dt))
                        return static_cast<qint8>(this->_reader->at(pos));
                    else
                        return static_cast<quint8>(this->_reader->at(pos));
                }

                case 16:
                {
                    if(DataType::isSigned(dt))
                        return this->_reader->readInt16(pos, DataType::byteOrder(dt));
                    else
                        return this->_reader->readUInt16(pos, DataType::byteOrder(dt));
                }

                case 32:
                {
                    if(DataType::isSigned(dt))
                        return this->_reader->readInt32(pos, DataType::byteOrder(dt));
                    else
                        return this->_reader->readUInt32(pos, DataType::byteOrder(dt));
                }

                case 64:
                {
                    if(DataType::isSigned(dt))
                        return this->_reader->readInt64(pos, DataType::byteOrder(dt));
                    else
                        return this->_reader->readUInt64(pos, DataType::byteOrder(dt));
                }

                default:
                    break;
            }
        }
        else if(dt == DataType::Character)
            return this->_reader->at(pos);

        const QMetaObject metaobj = DataType::staticMetaObject;
        QMetaEnum dtenum = metaobj.enumerator(metaobj.indexOfEnumerator("Type"));
        luaL_error(LuaState::instance(), "DataBuffer:readType(): Unsupported DataType '%s'", dtenum.valueToKey(dt));
        return 0;
    }

    void DataBuffer::writeType(lua_Integer pos, lua_Integer datatype, lua_Integer value)
    {
        DataType::Type dt = static_cast<DataType::Type>(datatype);
        pos += this->_baseoffset;

        if(DataType::isInteger(dt))
        {
            switch(DataType::bitWidth(dt))
            {
                case 8:
                {
                    if(DataType::isSigned(dt))
                        this->_writer->writeInt8(pos, static_cast<qint8>(value));
                    else
                        this->_writer->writeUInt8(pos, static_cast<quint8>(value));

                    return;
                }

                case 16:
                {
                    if(DataType::isSigned(dt))
                        this->_writer->writeInt16(pos, static_cast<qint16>(value), DataType::byteOrder(dt));
                    else
                        this->_writer->writeUInt16(pos, static_cast<quint16>(value), DataType::byteOrder(dt));

                    return;
                }

                case 32:
                {
                    if(DataType::isSigned(dt))
                        this->_writer->writeInt32(pos, static_cast<qint32>(value), DataType::byteOrder(dt));
                    else
                        this->_writer->writeUInt32(pos, static_cast<quint32>(value), DataType::byteOrder(dt));

                    return;
                }

                case 64:
                {
                    if(DataType::isSigned(dt))
                        this->_writer->writeInt64(pos, static_cast<qint64>(value), DataType::byteOrder(dt));
                    else
                        this->_writer->writeUInt64(pos, static_cast<quint64>(value), DataType::byteOrder(dt));

                    return;
                }

                default:
                    break;
            }
        }
        else if(dt == DataType::Character)
        {
            this->_writer->writeUInt8(pos, static_cast<quint8>(value));
            return;
        }

        const QMetaObject metaobj = DataType::staticMetaObject;
        QMetaEnum dtenum = metaobj.enumerator(metaobj.indexOfEnumerator("Type"));
        luaL_error(LuaState::instance(), "DataBuffer::writeType(): Unsupported DataType '%s'", dtenum.valueToKey(dt));
    }

    QString DataBuffer::readLine(lua_Integer pos)
    {
        QString s;
        char ch = '\0';

        for(qint64 i = this->_baseoffset + pos; i < this->_hexeditdata->length(); i++)
        {
            ch = static_cast<char>(this->_reader->at(i));

            if(ch == '\n' || ch == '\r')
                break;

            s.append(QChar(ch));
        }

        return s;
    }

    QString DataBuffer::readString(lua_Integer pos, lua_Integer maxlen)
    {
        return this->_reader->readString(this->_baseoffset + pos, maxlen);
    }

    void DataBuffer::writeString(lua_Integer pos, const QString &s)
    {
        this->_writer->replace(this->_baseoffset + pos, s.length(), s.toUtf8());
    }

    int DataBuffer::metaIndex(lua_State *l, lua_Integer key)
    {
        if((key < 0) || (key >= this->_hexeditdata->length()))
        {
            throw PrefException(QString("DataBuffer::metaIndex(): Index out of range (%1)").arg(key));
            return 0;
        }

        lua_pushinteger(l, this->_reader->at(this->_baseoffset + key));
        return 1;
    }
}
