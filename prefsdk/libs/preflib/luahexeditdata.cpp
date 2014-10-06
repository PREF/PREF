#include "luahexeditdata.h"

namespace PrefSDK
{
    LuaHexEditData::LuaHexEditData(QHexEditData* hexeditdata, QObject *parent): QObject(parent), _hexeditdata(hexeditdata)
    {
        this->_reader = new QHexEditDataReader(hexeditdata, this);
        this->_writer = new QHexEditDataWriter(hexeditdata, this);
    }

    QHexEditData *LuaHexEditData::hexEditData() const
    {
        return this->_hexeditdata;
    }

    lua_Integer LuaHexEditData::length() const
    {
        return this->_hexeditdata->length();
    }

    void LuaHexEditData::copyTo(QObject *luahexeditdata, lua_Integer start, lua_Integer end)
    {
        LuaHexEditData* luahexeditdataout = qobject_cast<LuaHexEditData*>(luahexeditdata);

        QHexEditDataDevice datain(this->_hexeditdata);
        QHexEditDataDevice dataout(luahexeditdataout->_hexeditdata);

        datain.open(QIODevice::ReadOnly);
        dataout.open(QIODevice::WriteOnly);

        datain.seek(start);
        dataout.write(datain.read(end - start));

        dataout.close();
        datain.close();

        luahexeditdataout->_hexeditdata->save();
    }

    lua_Integer LuaHexEditData::indexOf(const QString &s, lua_Integer startpos)
    {
        return this->_reader->indexOf(s.toUtf8(), startpos);
    }

    lua_Integer LuaHexEditData::readType(lua_Integer pos, lua_Integer datatype)
    {
        DataType::Type dt = static_cast<DataType::Type>(datatype);

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
        luaL_error(LuaState::instance(), "LuaHexEditData:readType(): Unsupported DataType '%s'", dtenum.valueToKey(dt));
        return 0;
    }

    void LuaHexEditData::writeType(lua_Integer pos, lua_Integer datatype, lua_Integer value)
    {
        DataType::Type dt = static_cast<DataType::Type>(datatype);

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
        luaL_error(LuaState::instance(), "LuaHexEditData:writeType(): Unsupported DataType '%s'", dtenum.valueToKey(dt));
    }

    QString LuaHexEditData::readLine(lua_Integer pos)
    {
        QString s;
        char ch = '\0';

        for(qint64 i = pos; i < this->_hexeditdata->length(); i++)
        {
            ch = static_cast<char>(this->_reader->at(i));

            if(ch == '\n' || ch == '\r')
                break;

            s.append(QChar(ch));
        }

        return s;
    }

    QString LuaHexEditData::readString(lua_Integer pos, lua_Integer maxlen)
    {
        return this->_reader->readString(pos, maxlen);
    }

    void LuaHexEditData::writeString(lua_Integer pos, const QString &s)
    {
        this->_writer->replace(pos, s.length(), s.toUtf8());
    }

    int LuaHexEditData::metaIndex(lua_State *l, lua_Integer key)
    {
        if(key >= 0 && key < this->_hexeditdata->length())
        {
            lua_pushinteger(l, this->_reader->at(key));
            return 1;
        }

        return 0;
    }
}
