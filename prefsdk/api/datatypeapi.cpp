#include "datatypeapi.h"

namespace PrefSDK
{
    namespace API
    {
        bool DataType_isInteger(uint64_t type)
        {
            return DataType::isInteger(static_cast<DataType::Type>(type));
        }

        bool DataType_isSigned(uint64_t type)
        {
            return DataType::isSigned(static_cast<DataType::Type>(type));
        }

        bool DataType_isString(uint64_t type)
        {
            return DataType::isString(static_cast<DataType::Type>(type));
        }

        bool DataType_isAscii(uint64_t type)
        {
            return DataType::isAscii(static_cast<DataType::Type>(type));
        }

        bool DataType_isUnicode(uint64_t type)
        {
            return DataType::isUnicode(static_cast<DataType::Type>(type));
        }

        bool DataType_isArray(uint64_t type)
        {
            return DataType::isArray(static_cast<DataType::Type>(type));
        }

        bool DataType_isLittleEndian(uint64_t type)
        {
            return DataType::isLittleEndian(static_cast<DataType::Type>(type));
        }

        bool DataType_isBigEndian(uint64_t type)
        {
            return DataType::isBigEndian(static_cast<DataType::Type>(type));
        }

        int DataType_sizeOf(uint64_t type)
        {
            return DataType::sizeOf(static_cast<DataType::Type>(type));
        }

        int DataType_byteOrder(uint64_t type)
        {
            return DataType::byteOrder(static_cast<DataType::Type>(type));
        }

        int DataType_bitWidth(uint64_t type)
        {
            return DataType::bitWidth(static_cast<DataType::Type>(type));
        }

        const char *DataType_stringValue(uint64_t type)
        {
            return DataType::stringValue(static_cast<DataType::Type>(type)).toUtf8().constData();
        }

        int DataType_byteWidth(uint64_t type)
        {
            return DataType::byteWidth(static_cast<DataType::Type>(type));
        }
    }
}
