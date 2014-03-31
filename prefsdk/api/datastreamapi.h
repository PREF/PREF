#ifndef PREFSDK_API_DATASTREAMAPI_H
#define PREFSDK_API_DATASTREAMAPI_H

#include <QtCore>
#include <cstdint>

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            QDataStream* DataStream_new(QByteArray* ba, int byteorder);
            void DataStream_delete(QDataStream* __this);

            uint8_t DataStream_readUInt8(QDataStream* __this);
            uint16_t DataStream_readUInt16(QDataStream* __this);
            uint32_t DataStream_readUInt32(QDataStream* __this);
            uint64_t DataStream_readUInt64(QDataStream* __this);

            int8_t DataStream_readInt8(QDataStream* __this);
            int16_t DataStream_readInt16(QDataStream* __this);
            int32_t DataStream_readInt32(QDataStream* __this);
            int64_t DataStream_readInt64(QDataStream* __this);
        }
    }
}

#endif // PREFSDK_API_DATASTREAMAPI_H
