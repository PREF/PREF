#ifndef PREFSDK_DATATYPE_H
#define PREFSDK_DATATYPE_H

#include <QtCore>

namespace PrefSDK
{
    class DataType
    {
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
                Invalid            = DataType::Undefined,

                UInt8              = DataType::Unsigned | DataType::Integer | 0x00000001,
                UInt16             = DataType::Unsigned | DataType::Integer | 0x00000002,
                UInt32             = DataType::Unsigned | DataType::Integer | 0x00000004,
                UInt64             = DataType::Unsigned | DataType::Integer | 0x00000008,

                Int8               = DataType::Integer | 0x00000001,
                Int16              = DataType::Integer | 0x00000002,
                Int32              = DataType::Integer | 0x00000004,
                Int64              = DataType::Integer | 0x00000008,

                AsciiCharacter     = DataType::Characters | DataType::Ascii,
                UnicodeCharacter   = DataType::Characters | DataType::Unicode,
                Character          = DataType::AsciiCharacter,

                Array              = DataType::Vector,
                AsciiString        = DataType::Vector | DataType::Characters | DataType::Ascii,
                UnicodeString      = DataType::Vector | DataType::Characters | DataType::Unicode,

                Blob               = DataType::Undefined,

            };

        public:
            static bool isInteger(DataType::Type type);
            static bool isSigned(DataType::Type type);
            static bool isString(DataType::Type type);
            static bool isAscii(DataType::Type type);
            static bool isUnicode(DataType::Type type);
            static bool isArray(DataType::Type type);
            static int sizeOf(DataType::Type type);
            static int byteWidth(DataType::Type type);
            static QString stringValue(DataType::Type type);
    };
}
#endif // PREFSDK_DATATYPE_H
