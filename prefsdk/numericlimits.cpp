#include "numericlimits.h"

namespace PrefSDK
{
    NumericLimits::NumericLimits()
    {
    }

    bool NumericLimits::willOverflow(QHexEditData *hexeditdata, quint64 pos, DataType::Type datatype)
    {
        switch(datatype)
        {
            case DataType::UInt8:
                return NumericLimits::willOverflowT<quint8>(hexeditdata, pos, static_cast<DataType::Type>(datatype));

            case DataType::UInt16:
            case DataType::UInt16_LE:
            case DataType::UInt16_BE:
                return NumericLimits::willOverflowT<quint16>(hexeditdata, pos, static_cast<DataType::Type>(datatype));

            case DataType::UInt32:
            case DataType::UInt32_LE:
            case DataType::UInt32_BE:
                return NumericLimits::willOverflowT<quint32>(hexeditdata, pos, static_cast<DataType::Type>(datatype));

            case DataType::UInt64_LE:
            case DataType::UInt64_BE:
                return NumericLimits::willOverflowT<quint64>(hexeditdata, pos, static_cast<DataType::Type>(datatype));

            case DataType::Int8:
                return NumericLimits::willOverflowT<qint8>(hexeditdata, pos, static_cast<DataType::Type>(datatype));

            case DataType::Int16:
            case DataType::Int16_LE:
            case DataType::Int16_BE:
                return NumericLimits::willOverflowT<qint16>(hexeditdata, pos, static_cast<DataType::Type>(datatype));

            case DataType::Int32:
            case DataType::Int32_LE:
            case DataType::Int32_BE:
                return NumericLimits::willOverflowT<qint32>(hexeditdata, pos, static_cast<DataType::Type>(datatype));

            case DataType::Int64:
            case DataType::Int64_LE:
            case DataType::Int64_BE:
                return NumericLimits::willOverflowT<qint64>(hexeditdata, pos, static_cast<DataType::Type>(datatype));

            default:
                break;
        }

        return false;
    }
}
