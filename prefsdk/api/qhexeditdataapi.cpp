#include "qhexeditdataapi.h"

namespace PrefSDK
{
    namespace API
    {
        void QHexEditData_copyTo(QHexEditData *__this, QHexEditData *hexeditdata)
        {
            __this->device()->seek(0);
            QByteArray ba = __this->device()->readAll();

            hexeditdata->device()->seek(0);
            hexeditdata->device()->write(ba);

            __this->device()->seek(0);
            hexeditdata->device()->seek(0);
        }

        int64_t QHexEditData_length(QHexEditData *__this)
        {
            return __this->length();
        }

        uint8_t QHexEditData_readUInt8(QHexEditData *__this, uint64_t pos)
        {
           return static_cast<uint8_t>(__this->at(pos));
        }

        uint16_t QHexEditData_readUInt16(QHexEditData* __this, uint64_t pos, int endian)
        {
            return static_cast<uint16_t>(__this->readUInt16(pos, static_cast<QSysInfo::Endian>(endian)));
        }

        uint32_t QHexEditData_readUInt32(QHexEditData* __this, uint64_t pos, int endian)
        {
            return static_cast<int32_t>(__this->readUInt32(pos, static_cast<QSysInfo::Endian>(endian)));
        }

        uint64_t QHexEditData_readUInt64(QHexEditData *__this, uint64_t pos, int endian)
        {
            return static_cast<uint64_t>(__this->readUInt64(pos, static_cast<QSysInfo::Endian>(endian)));
        }

        int8_t QHexEditData_readInt8(QHexEditData *__this, uint64_t pos)
        {
            return static_cast<int8_t>(__this->at(pos));
        }

        int16_t QHexEditData_readInt16(QHexEditData* __this, uint64_t pos, int endian)
        {
            return static_cast<int16_t>(__this->readInt16(pos, static_cast<QSysInfo::Endian>(endian)));
        }

        int32_t QHexEditData_readInt32(QHexEditData* __this, uint64_t pos, int endian)
        {
            return static_cast<int32_t>(__this->readInt32(pos, static_cast<QSysInfo::Endian>(endian)));
        }

        int64_t QHexEditData_readInt64(QHexEditData *__this, uint64_t pos, int endian)
        {
            return static_cast<int64_t>(__this->readInt64(pos, static_cast<QSysInfo::Endian>(endian)));
        }
    }
}
