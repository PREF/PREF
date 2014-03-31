#ifndef DATATYPE_H
#define DATATYPE_H

#include <QtCore>
#include "qlua.h"

namespace PrefSDK
{
    class DataType
    {
        private:
            DataType();

        public:
            static void load(lua_State* l);

        public: /* Types */
            static lua_Integer invalid();
            static lua_Integer uint8();
            static lua_Integer uint16();
            static lua_Integer uint32();
            static lua_Integer uint64();
            static lua_Integer int8();
            static lua_Integer int16();
            static lua_Integer int32();
            static lua_Integer int64();
            static lua_Integer array();
            static lua_Integer blob();
            static lua_Integer character();
            static lua_Integer asciiCharacter();
            static lua_Integer unicodeCharacter();
            static lua_Integer asciiString();
            static lua_Integer unicodeString();


        public: /* Methods */
            static bool isInteger(lua_Integer datatype);
            static bool isSigned(lua_Integer datatype);
            static bool isString(lua_Integer datatype);
            static bool isAscii(lua_Integer datatype);
            static bool isUnicode(lua_Integer datatype);
            static bool isArray(lua_Integer datatype);
            static lua_Integer sizeOf(lua_Integer type);
            static lua_Integer byteWidth(lua_Integer datatype);
            static QString stringValue(lua_Integer datatype);

        private:
            static LuaTable::Ptr _datatypetable;
    };
}
#endif // DATATYPE_H
