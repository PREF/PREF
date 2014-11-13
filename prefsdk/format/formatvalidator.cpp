#include "formatvalidator.h"

namespace PrefSDK
{
    FormatValidator::FormatValidator(QHexEditData *hexeditdata, qint64 baseoffset, QObject *parent): QObject(parent), _hexeditdata(hexeditdata), _baseoffset(baseoffset), _validated(true)
    {
        this->_reader = new QHexEditDataReader(hexeditdata, this);
        this->_databuffer = new DataBuffer(hexeditdata, baseoffset, this);
    }

    bool FormatValidator::validated() const
    {
        return this->_validated;
    }

    void FormatValidator::setValidated(bool b)
    {
        this->_validated = b;
    }

    PrefSDK::DataBuffer *FormatValidator::buffer()
    {
        return this->_databuffer;
    }

    bool FormatValidator::checkAscii(lua_Integer offset, const QString &expected, bool throwerr)
    {
        QByteArray dataval = this->_reader->read(this->_baseoffset + offset, expected.length());
        this->_validated = (QString::compare(expected, QString(dataval)) == 0);

        if(!this->_validated && throwerr)
            this->expect(expected, QString(dataval), QString::number(offset, 16).toUpper().append("h"));

        return this->_validated;
    }

    bool FormatValidator::checkType(lua_Integer offset, lua_Integer expected, lua_Integer datatype, bool throwerr)
    {
        DataType::Type type = static_cast<DataType::Type>(datatype);

        switch(DataType::bitWidth(type))
        {
            case 8:
            {
                if(DataType::isSigned(type))
                    return this->checkInt8(offset, static_cast<qint8>(expected), throwerr);

                return this->checkUInt8(offset, static_cast<quint8>(expected), throwerr);
            }

            case 16:
            {
                if(DataType::isSigned(type))
                    return this->checkInt16(offset, static_cast<qint16>(expected), DataType::byteOrder(type), throwerr);

                return this->checkUInt16(offset, static_cast<quint16>(expected), DataType::byteOrder(type), throwerr);
            }

            case 32:
            {
                if(DataType::isSigned(type))
                    return this->checkInt32(offset, static_cast<qint32>(expected), DataType::byteOrder(type), throwerr);

                return this->checkUInt32(offset, static_cast<quint32>(expected), DataType::byteOrder(type), throwerr);
            }

            case 64:
            {
                if(DataType::isSigned(type))
                    return this->checkInt64(offset, static_cast<qint64>(expected), DataType::byteOrder(type), throwerr);

                return this->checkUInt64(offset, static_cast<quint64>(expected), DataType::byteOrder(type), throwerr);
            }

            default:
                throw PrefException("validator.checkType(): Invalid DataType");
                return false;
        }

        return false;
    }

    bool FormatValidator::checkInt8(lua_Integer offset, qint8 expected, bool throwerr)
    {
        qint8 dataval = static_cast<qint8>(this->_reader->at(this->_baseoffset + offset));
        this->_validated = (dataval == expected);

        if(!this->_validated && throwerr)
        {
            this->expect(QString("%1").arg(expected, 2, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString("%1").arg(dataval, 2, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString::number(offset, 16).toUpper().append("h"));
        }

        return this->_validated;
    }

    bool FormatValidator::checkInt16(lua_Integer offset, qint16 expected, int byteorder, bool throwerr)
    {
        qint16 dataval = this->_reader->readInt16(this->_baseoffset + offset, static_cast<QSysInfo::Endian>(byteorder));
        this->_validated = (dataval == expected);

        if(!this->_validated && throwerr)
        {
            this->expect(QString("%1").arg(expected, 4, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString("%1").arg(dataval, 4, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString::number(offset, 16).toUpper().append("h"));
        }

        return this->_validated;
    }

    bool FormatValidator::checkInt32(lua_Integer offset, qint32 expected, int byteorder, bool throwerr)
    {
        qint32 dataval = this->_reader->readInt32(this->_baseoffset + offset, static_cast<QSysInfo::Endian>(byteorder));
        this->_validated = (dataval == expected);

        if(!this->_validated && throwerr)
        {
            this->expect(QString("%1").arg(expected, 8, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString("%1").arg(dataval, 8, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString::number(offset, 16).toUpper().append("h"));
        }

        return this->_validated;
    }

    bool FormatValidator::checkInt64(lua_Integer offset, qint64 expected, int byteorder, bool throwerr)
    {
        qint64 dataval = this->_reader->readInt64(this->_baseoffset + offset, static_cast<QSysInfo::Endian>(byteorder));
        this->_validated = (dataval == expected);

        if(!this->_validated && throwerr)
        {
            this->expect(QString("%1").arg(static_cast<qint64>(expected), 16, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString("%1").arg(static_cast<qint64>(dataval), 16, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString::number(offset, 16).toUpper().append("h"));
        }

        return this->_validated;
    }

    bool FormatValidator::checkUInt8(lua_Integer offset, quint8 expected, bool throwerr)
    {
        quint8 dataval = static_cast<quint8>(this->_reader->at(this->_baseoffset + offset));
        this->_validated = (dataval == expected);

        if(!this->_validated && throwerr)
        {
            this->expect(QString("%1").arg(expected, 2, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString("%1").arg(dataval, 2, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString::number(offset, 16).toUpper().append("h"));
        }

        return this->_validated;
    }

    bool FormatValidator::checkUInt16(lua_Integer offset, quint16 expected, int byteorder, bool throwerr)
    {
        quint16 dataval = this->_reader->readUInt16(this->_baseoffset + offset, static_cast<QSysInfo::Endian>(byteorder));
        this->_validated = (dataval == expected);

        if(!this->_validated && throwerr)
        {
            this->expect(QString("%1").arg(expected, 4, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString("%1").arg(dataval, 4, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString::number(offset, 16).toUpper().append("h"));
        }

        return this->_validated;
    }

    bool FormatValidator::checkUInt32(lua_Integer offset, quint32 expected, int byteorder, bool throwerr)
    {
        quint16 dataval = this->_reader->readUInt32(this->_baseoffset + offset, static_cast<QSysInfo::Endian>(byteorder));
        this->_validated = (dataval == expected);

        if(!this->_validated && throwerr)
        {
            this->expect(QString("%1").arg(expected, 8, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString("%1").arg(dataval, 8, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString::number(offset, 16).toUpper().append("h"));
        }

        return this->_validated;
    }

    bool FormatValidator::checkUInt64(lua_Integer offset, quint64 expected, int byteorder, bool throwerr)
    {
        quint16 dataval = this->_reader->readUInt64(this->_baseoffset + offset, static_cast<QSysInfo::Endian>(byteorder));
        this->_validated = (dataval == expected);

        if(!this->_validated && throwerr)
        {
            this->expect(QString("%1").arg(static_cast<quint64>(expected), 16, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString("%1").arg(static_cast<quint64>(dataval), 16, 16, QLatin1Char('0')).toUpper().append("h"),
                         QString::number(offset, 16).toUpper().append("h"));
        }

        return this->_validated;
    }

    void FormatValidator::expect(const QString &expect, const QString &found, const QString& at)
    {
        luaL_error(LuaState::instance(), "Expected '%s' but found '%s' at %s", expect.toUtf8().constData(), found.toUtf8().constData(), at.toUtf8().constData());
    }

    void FormatValidator::error(const QString &msg)
    {
        luaL_error(LuaState::instance(), msg.toUtf8().constData());
    }
}
