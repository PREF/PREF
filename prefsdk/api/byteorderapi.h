#ifndef PREFSDK_API_BYTEORDERAPI_H
#define PREFSDK_API_BYTEORDERAPI_H

#include <QtCore>
#include <QtEndian>
#include "prefsdk/datatype.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C" const int ByteOrder_LittleEndian = QSysInfo::LittleEndian;
        extern "C" const int ByteOrder_BigEndian = QSysInfo::BigEndian;
        extern "C" const int ByteOrder_PlatformEndian = QSysInfo::ByteOrder;
    }
}

#endif // PREFSDK_API_BYTEORDERAPI_H
