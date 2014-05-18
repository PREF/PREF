#ifndef PREFSDK_API_QHEXEDITDATAAPI_H
#define PREFSDK_API_QHEXEDITDATAAPI_H

#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatadevice.h"
#include "qhexedit/qhexeditdatareader.h"
#include "qhexedit/qhexeditdatawriter.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            QHexEditDataReader* QHexEditData_createReader(QHexEditData* __this);
            QHexEditDataWriter* QHexEditData_createWriter(QHexEditData* __this);
            void QHexEditData_copyTo(QHexEditData* __this, QHexEditData* hexeditdata, qint64 start, qint64 end);
            int64_t QHexEditData_length(QHexEditData* __this);

            int64_t QHexEditDataReader_indexOf(QHexEditDataReader* __this, int64_t pos, const char* s);

            char QHexEditDataReader_readAsciiChar(QHexEditDataReader* __this, int64_t pos);
            const char* QHexEditDataReader_readString(QHexEditDataReader* __this, int64_t pos, int64_t len);
            const char* QHexEditDataReader_readLine(QHexEditDataReader *__this, int64_t pos);

            uint8_t QHexEditDataReader_readUInt8(QHexEditDataReader* __this, uint64_t pos);
            uint16_t QHexEditDataReader_readUInt16(QHexEditDataReader* __this, uint64_t pos, int endian);
            uint32_t QHexEditDataReader_readUInt32(QHexEditDataReader* __this, uint64_t pos, int endian);
            uint64_t QHexEditDataReader_readUInt64(QHexEditDataReader* __this, uint64_t pos, int endian);

            int8_t QHexEditDataReader_readInt8(QHexEditDataReader* __this, uint64_t pos);
            int16_t QHexEditDataReader_readInt16(QHexEditDataReader* __this, uint64_t pos, int endian);
            int32_t QHexEditDataReader_readInt32(QHexEditDataReader* __this, uint64_t pos, int endian);
            int64_t QHexEditDataReader_readInt64(QHexEditDataReader* __this, uint64_t pos, int endian);
        }
    }
}

#endif // PREFSDK_API_QHEXEDITDATAAPI_H
