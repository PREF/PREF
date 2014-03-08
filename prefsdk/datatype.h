#ifndef DATATYPE_H
#define DATATYPE_H

#include <QtCore>
#include <limits>
#include "qhexedit/qhexeditdata.h"
#include "qlua.h"

namespace PrefSDK
{
    class DataType: public LuaCTable
    {
        public:
            typedef std::shared_ptr<DataType> Ptr;

        private:
            enum TypeDefinitions
            {
                Undefined    = 0x00000000,
                Unsigned     = 0x10000000,
                Vector       = 0x20000000,

                Integer      = 0x00010000,
                Characters   = 0x00020000,

                Ascii        = 0x00100000,
                Unicode      = 0x00200000,
            };

        public:
            enum Type
            {
                Invalid       = DataType::Undefined,

                UInt8         = DataType::Unsigned | DataType::Integer | 0x00000001,
                UInt16        = DataType::Unsigned | DataType::Integer | 0x00000002,
                UInt32        = DataType::Unsigned | DataType::Integer | 0x00000004,
                UInt64        = DataType::Unsigned | DataType::Integer | 0x00000008,

                Int8          = DataType::Integer | 0x00000001,
                Int16         = DataType::Integer | 0x00000002,
                Int32         = DataType::Integer | 0x00000004,
                Int64         = DataType::Integer | 0x00000008,

                AsciiChar     = DataType::Characters | DataType::Ascii,
                UnicodeChar   = DataType::Characters | DataType::Unicode,
                Char          = DataType::AsciiChar,

                List          = DataType::Vector,
                AsciiString   = DataType::Vector | DataType::Characters | DataType::Ascii,
                UnicodeString = DataType::Vector | DataType::Characters | DataType::Unicode,

                Blob          = DataType::Undefined,

            };

        private:
            DataType(lua_State* l);

        public:
            static DataType::Ptr create(lua_State* l);
            static bool isInteger(DataType::Type type);
            static bool isSigned(DataType::Type type);
            static bool isString(DataType::Type type);
            static bool isAscii(DataType::Type type);
            static bool isUnicode(DataType::Type type);
            static bool isList(DataType::Type type);
            static QString stringValue(DataType::Type type);
            static int byteLength(DataType::Type type);

        lua_public:
            static lua_Integer sizeOf(lua_Integer type);

        lua_private:
            static bool isInteger(lua_Integer type);
            static bool isSigned(lua_Integer type);
            static bool isString(lua_Integer type);
            static bool isAscii(lua_Integer type);
            static bool isUnicode(lua_Integer type);
            static bool isList(lua_Integer type);
    };

    namespace Data
    {
        template<DataType::Type TYPE> struct ToType
        {

        };

        template<> struct ToType<DataType::UInt8>
        {
            typedef quint8 Type;
        };

        template<> struct ToType<DataType::UInt16>
        {
            typedef quint16 Type;
        };

        template<> struct ToType<DataType::UInt32>
        {
            typedef quint32 Type;
        };

        template<> struct ToType<DataType::UInt64>
        {
            typedef quint64 Type;
        };

        template<> struct ToType<DataType::Int8>
        {
            typedef qint8 Type;
        };

        template<> struct ToType<DataType::Int16>
        {
            typedef qint16 Type;
        };

        template<> struct ToType<DataType::Int32>
        {
            typedef qint32 Type;
        };

        template<> struct ToType<DataType::Int64>
        {
            typedef qint64 Type;
        };

        template<> struct ToType<DataType::AsciiChar>
        {
            typedef char Type;
        };

        template<> struct ToType<DataType::UnicodeChar>
        {
            typedef wchar_t Type;
        };
    }
}
#endif // DATATYPE_H
