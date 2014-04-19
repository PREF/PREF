#ifndef PREFSDK_API_QHEXEDITDATAAPI_H
#define PREFSDK_API_QHEXEDITDATAAPI_H

#include "qhexedit/qhexeditdata.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            void QHexEditData_copyTo(QHexEditData* __this, QHexEditData* hexeditdata);
            int64_t QHexEditData_length(QHexEditData* __this);

            uint8_t QHexEditData_readUInt8(QHexEditData* __this, uint64_t pos);
            uint16_t QHexEditData_readUInt16(QHexEditData* __this, uint64_t pos, int endian);
            uint32_t QHexEditData_readUInt32(QHexEditData* __this, uint64_t pos, int endian);
            uint64_t QHexEditData_readUInt64(QHexEditData* __this, uint64_t pos, int endian);

            int8_t QHexEditData_readInt8(QHexEditData* __this, uint64_t pos);
            int16_t QHexEditData_readInt16(QHexEditData* __this, uint64_t pos, int endian);
            int32_t QHexEditData_readInt32(QHexEditData* __this, uint64_t pos, int endian);
            int64_t QHexEditData_readInt64(QHexEditData* __this, uint64_t pos, int endian);
        }
    }
}

#endif // PREFSDK_API_QHEXEDITDATAAPI_H
