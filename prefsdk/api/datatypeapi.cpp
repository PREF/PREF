#include "datatypeapi.h"


bool PrefSDK::API::DataType_isInteger(u_int64_t type)
{
    return DataType::isInteger(static_cast<DataType::Type>(type));
}


bool PrefSDK::API::DataType_isSigned(u_int64_t type)
{
    return DataType::isSigned(static_cast<DataType::Type>(type));
}

bool PrefSDK::API::DataType_isString(u_int64_t type)
{
    return DataType::isString(static_cast<DataType::Type>(type));
}

bool PrefSDK::API::DataType_isAscii(u_int64_t type)
{
    return DataType::isAscii(static_cast<DataType::Type>(type));
}

bool PrefSDK::API::DataType_isUnicode(u_int64_t type)
{
    return DataType::isUnicode(static_cast<DataType::Type>(type));
}

bool PrefSDK::API::DataType_isArray(u_int64_t type)
{
    return DataType::isArray(static_cast<DataType::Type>(type));
}

int PrefSDK::API::DataType_sizeOf(u_int64_t type)
{
    return DataType::sizeOf(static_cast<DataType::Type>(type));
}

int PrefSDK::API::DataType_byteWidth(u_int64_t type)
{
    return DataType::byteWidth(static_cast<DataType::Type>(type));
}

const char *PrefSDK::API::DataType_stringValue(u_int64_t type)
{
    return DataType::stringValue(static_cast<DataType::Type>(type)).toUtf8().constData();
}
