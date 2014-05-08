#ifndef PREFSDK_NUMERICLIMITS_H
#define PREFSDK_NUMERICLIMITS_H

#include <limits>
#include "prefsdk/datatype.h"
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"

namespace PrefSDK
{
    class NumericLimits
    {
        private:
            NumericLimits();

        private:
            template<typename TYPE> static bool willOverflowT(QHexEditData* hexeditdata, uint64_t pos, DataType::Type datatype)
            {
                TYPE val;
                QHexEditDataReader reader(hexeditdata);
                QByteArray ba = reader.read(pos, DataType::sizeOf(datatype));

                QDataStream ds(ba);

                if(DataType::byteOrder(datatype) == QSysInfo::LittleEndian)
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

        public:
            static bool willOverflow(QHexEditData* hexeditdata, uint64_t pos, DataType::Type datatype);
    };
}

#endif // PREFSDK_NUMERICLIMITS_H
