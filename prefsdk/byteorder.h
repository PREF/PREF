#ifndef PREFSDK_BYTEORDER_H
#define PREFSDK_BYTEORDER_H

#include <QtCore>
#include "qlua.h"

namespace PrefSDK
{
    class ByteOrder
    {
        private:
            ByteOrder();

        public:
            static void load(lua_State* l);
            static QSysInfo::Endian qEndian(lua_Integer sdkendian);
            static lua_Integer sdkEndian(QSysInfo::Endian qendian);

        private:
            static lua_Integer _platformendian;
            static lua_Integer _littleendian;
            static lua_Integer _bigendian;
    };
}
#endif // PREFSDK_BYTEORDER_H
