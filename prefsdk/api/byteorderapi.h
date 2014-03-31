#ifndef PREFSDK_API_BYTEORDERAPI_H
#define PREFSDK_API_BYTEORDERAPI_H

#include <QtCore>

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            const int ByteOrder_LittleEndian = QSysInfo::LittleEndian;
            const int ByteOrder_BigEndian = QSysInfo::BigEndian;
            const int ByteOrder_PlatformEndian = QSysInfo::ByteOrder;
        }
    }
}

#endif // PREFSDK_API_BYTEORDERAPI_H
