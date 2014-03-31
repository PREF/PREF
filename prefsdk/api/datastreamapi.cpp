#include "datastreamapi.h"

namespace PrefSDK
{
    namespace API
    {
        QDataStream *DataStream_new(QByteArray *ba, int byteorder)
        {
            QDataStream* ds = new QDataStream(ba, QIODevice::ReadWrite);

            if(static_cast<QSysInfo::Endian>(byteorder) == QSysInfo::LittleEndian)
                ds->setByteOrder(QDataStream::LittleEndian);
            else
                ds->setByteOrder(QDataStream::BigEndian);

            return ds;
        }

        void DataStream_delete(QDataStream *__this)
        {
            delete __this;
        }

        uint8_t DataStream_readUInt8(QDataStream *__this)
        {
            uint8_t v;
            (*__this) >> v;
            return v;
        }

        uint16_t DataStream_readUInt16(QDataStream *__this)
        {
            uint16_t v;
            (*__this) >> v;
            return v;
        }

        uint32_t DataStream_readUInt32(QDataStream *__this)
        {
            uint32_t v;
            (*__this) >> v;
            return v;
        }

        uint64_t DataStream_readUInt64(QDataStream *__this)
        {
            quint64 v;
            (*__this) >> v;
            return static_cast<uint64_t>(v);
        }

        int8_t DataStream_readInt8(QDataStream *__this)
        {
            int8_t v;
            (*__this) >> v;
            return v;
        }

        int16_t DataStream_readInt16(QDataStream *__this)
        {
            int16_t v;
            (*__this) >> v;
            return v;
        }

        int32_t DataStream_readInt32(QDataStream *__this)
        {
            int32_t v;
            (*__this) >> v;
            return v;
        }

        int64_t DataStream_readInt64(QDataStream *__this)
        {
            qint64 v;
            (*__this) >> v;
            return static_cast<int64_t>(v);
        }
    }
}
