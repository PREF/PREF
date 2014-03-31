#include "numericlimitsapi.h"

bool PrefSDK::API::NumericLimits_willOverflow(QHexEditData *hexeditdata, int64_t pos, int datatype, int byteorder)
{
    switch(datatype)
    {
        case DataType::UInt8:
            return PrivateAPI::willOverflowT<uint8_t>(hexeditdata, pos, static_cast<DataType::Type>(datatype), static_cast<QSysInfo::Endian>(byteorder));

        case DataType::UInt16:
            return PrivateAPI::willOverflowT<uint16_t>(hexeditdata, pos, static_cast<DataType::Type>(datatype), static_cast<QSysInfo::Endian>(byteorder));

        case DataType::UInt32:
            return PrivateAPI::willOverflowT<uint32_t>(hexeditdata, pos, static_cast<DataType::Type>(datatype), static_cast<QSysInfo::Endian>(byteorder));

        case DataType::UInt64:
            return PrivateAPI::willOverflowT<quint64>(hexeditdata, pos, static_cast<DataType::Type>(datatype), static_cast<QSysInfo::Endian>(byteorder));

        case DataType::Int8:
            return PrivateAPI::willOverflowT<int8_t>(hexeditdata, pos, static_cast<DataType::Type>(datatype), static_cast<QSysInfo::Endian>(byteorder));

        case DataType::Int16:
            return PrivateAPI::willOverflowT<int16_t>(hexeditdata, pos, static_cast<DataType::Type>(datatype), static_cast<QSysInfo::Endian>(byteorder));

        case DataType::Int32:
            return PrivateAPI::willOverflowT<int32_t>(hexeditdata, pos, static_cast<DataType::Type>(datatype), static_cast<QSysInfo::Endian>(byteorder));

        case DataType::Int64:
            return PrivateAPI::willOverflowT<qint64>(hexeditdata, pos, static_cast<DataType::Type>(datatype), static_cast<QSysInfo::Endian>(byteorder));

        default:
            break;
    }

    return false;
}
