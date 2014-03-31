#ifndef NUMERICLIMITSAPI_H
#define NUMERICLIMITSAPI_H

#include <limits>
#include "prefsdk/datatype.h"
#include "qhexedit/qhexeditdata.h"

namespace PrefSDK
{
    namespace PrivateAPI
    {
        template<typename TYPE> bool willOverflowT(QHexEditData* hexeditdata, int64_t pos, DataType::Type datatype, QSysInfo::Endian byteorder)
        {
            TYPE val;
            QByteArray ba = hexeditdata->read(pos, DataType::sizeOf(datatype));

            QDataStream ds(ba);

            if(byteorder == QSysInfo::LittleEndian)
                ds.setByteOrder(QDataStream::LittleEndian);
            else
                ds.setByteOrder(QDataStream::BigEndian);

            ds >> val;

            if(std::numeric_limits<TYPE>::is_signed)
            {
                return static_cast<std::uintmax_t>(val) > static_cast<std::uintmax_t>(INTMAX_MAX) ||
                       static_cast<std::intmax_t>(val) < static_cast<std::intmax_t>(std::numeric_limits<TYPE>::min()) ||
                       static_cast<std::intmax_t>(val) > static_cast<std::intmax_t>(std::numeric_limits<TYPE>::max());
            }

            return (val < 0) || (val > static_cast<std::uintmax_t>(std::numeric_limits<TYPE>::max()));
        }
    }

    namespace API
    {
        extern "C"
        {
            bool NumericLimits_willOverflow(QHexEditData* hexeditdata, int64_t pos, int datatype, int byteorder);
        }
    }
}

#endif // NUMERICLIMITSAPI_H
