#include "qhexeditdataapi.h"

namespace PrefSDK
{
    namespace API
    {
        QHexEditDataReader *QHexEditData_createReader(QHexEditData *__this)
        {
            return new QHexEditDataReader(__this);
        }

        QHexEditDataWriter *QHexEditData_createWriter(QHexEditData *__this)
        {
            return new QHexEditDataWriter(__this);
        }

        int64_t QHexEditData_length(QHexEditData *__this)
        {
            return __this->length();
        }

        void QHexEditData_copyTo(QHexEditData *__this, QHexEditData *hexeditdata, qint64 start, qint64 end)
        {
            QHexEditDataDevice datain(__this);
            QHexEditDataDevice dataout(hexeditdata);

            datain.open(QIODevice::ReadOnly);
            dataout.open(QIODevice::WriteOnly);

            datain.seek(start);
            dataout.write(datain.read(end - start));

            dataout.close();
            datain.close();
        }

        int64_t QHexEditDataReader_indexOf(QHexEditDataReader *__this, int64_t pos, const char *s)
        {
            QString str = QString::fromUtf8(s);
            return __this->indexOf(str.toUtf8(), pos);
        }

        char QHexEditDataReader_readAsciiChar(QHexEditDataReader *__this, int64_t pos)
        {
            return static_cast<char>(__this->at(pos));
        }

        const char *QHexEditDataReader_readString(QHexEditDataReader *__this, int64_t pos, int64_t len)
        {
            if(len == -1)
                return __this->readString(pos).toUtf8().constData();

            QByteArray ba = __this->read(pos, len);
            return QString(ba).toUtf8().constData();
        }

        const char *QHexEditDataReader_readLine(QHexEditDataReader *__this, int64_t pos)
        {
            QString s;

            char ch = '\0';

            for(int64_t i = pos; i < __this->hexEditData()->length(); i++)
            {
                ch = static_cast<char>(__this->at(i));

                if(ch == '\n' || ch == '\r')
                    break;

                s.append(QChar(ch));
            }

            return s.toUtf8().constData();
        }

        uint8_t QHexEditDataReader_readUInt8(QHexEditDataReader *__this, uint64_t pos)
        {
           return static_cast<uint8_t>(__this->at(pos));
        }

        uint16_t QHexEditDataReader_readUInt16(QHexEditDataReader* __this, uint64_t pos, int endian)
        {
            return static_cast<uint16_t>(__this->readUInt16(pos, static_cast<QSysInfo::Endian>(endian)));
        }

        uint32_t QHexEditDataReader_readUInt32(QHexEditDataReader* __this, uint64_t pos, int endian)
        {
            return static_cast<int32_t>(__this->readUInt32(pos, static_cast<QSysInfo::Endian>(endian)));
        }

        uint64_t QHexEditDataReader_readUInt64(QHexEditDataReader* __this, uint64_t pos, int endian)
        {
            return static_cast<uint64_t>(__this->readUInt64(pos, static_cast<QSysInfo::Endian>(endian)));
        }

        int8_t QHexEditDataReader_readInt8(QHexEditDataReader* __this, uint64_t pos)
        {
            return static_cast<int8_t>(__this->at(pos));
        }

        int16_t QHexEditDataReader_readInt16(QHexEditDataReader* __this, uint64_t pos, int endian)
        {
            return static_cast<int16_t>(__this->readInt16(pos, static_cast<QSysInfo::Endian>(endian)));
        }

        int32_t QHexEditDataReader_readInt32(QHexEditDataReader* __this, uint64_t pos, int endian)
        {
            return static_cast<int32_t>(__this->readInt32(pos, static_cast<QSysInfo::Endian>(endian)));
        }

        int64_t QHexEditDataReader_readInt64(QHexEditDataReader* __this, uint64_t pos, int endian)
        {
            return static_cast<int64_t>(__this->readInt64(pos, static_cast<QSysInfo::Endian>(endian)));
        }
    }
}
