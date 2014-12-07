#ifndef PREFSDK_BYTEARRAY_H
#define PREFSDK_BYTEARRAY_H

#include <lua.hpp>
#include "prefsdk/prefexception.h"
#include <QHash>
#include <QByteArray>
#include <QDebug>

namespace PrefSDK
{
    class ByteArray
    {
        private:
            ByteArray();

        private:
            static int metaIndex(lua_State* l);
            static int metaNewIndex(lua_State* l);
            static int bytearray_append(lua_State* l);
            static int bytearray_length(lua_State* l);

        private:
            static void pushMetaTable(lua_State* l);

        public:
            static void push(lua_State* l, const QByteArray &bytearray);
    };
}

#endif // PREFSDK_BYTEARRAY_H
