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
                Undefined      = 0x00000000,
                Integer        = 0x10000000,
                Characters     = 0x20000000,
                Vector         = 0x40000000,

                Unsigned       = 0x01000000, /*  For Integers  */
                Signed         = 0x02000000, /*  For Integers  */

                Ascii          = 0x01000000, /* For Characters */
                Unicode        = 0x02000000, /* For Characters */

                PlatformEndian = 0x00100000, /*  For Integers  */
                LittleEndian   = 0x00200000, /*  For Integers  */
                BigEndian      = 0x00400000, /*  For Integers  */
            };

        private:
            enum IntegerType
            {
                Unsigned8Bit  = DataType::Integer | DataType::Unsigned | 0x00000001,
                Unsigned16Bit = DataType::Integer | DataType::Unsigned | 0x00000002,
                Unsigned32Bit = DataType::Integer | DataType::Unsigned | 0x00000004,
                Unsigned64Bit = DataType::Integer | DataType::Unsigned | 0x00000008,

                Signed8Bit    = DataType::Integer | DataType::Signed | 0x00000001,
                Signed16Bit   = DataType::Integer | DataType::Signed | 0x00000002,
                Signed32Bit   = DataType::Integer | DataType::Signed | 0x00000004,
                Signed64Bit   = DataType::Integer | DataType::Signed | 0x00000008,
            };

        public:
            enum Type
            {
                Invalid            = DataType::Undefined,

                /*                 Platform Dependent Integers                        */
                UInt8                 = DataType::PlatformEndian | DataType::Unsigned8Bit,
                UInt16                = DataType::PlatformEndian | DataType::Unsigned16Bit,
                UInt32                = DataType::PlatformEndian | DataType::Unsigned32Bit,
                UInt64                = DataType::PlatformEndian | DataType::Unsigned64Bit,

                Int8                  = DataType::PlatformEndian | DataType::Signed8Bit,
                Int16                 = DataType::PlatformEndian | DataType::Signed16Bit,
                Int32                 = DataType::PlatformEndian | DataType::Signed32Bit,
                Int64                 = DataType::PlatformEndian | DataType::Signed64Bit,

                /*                   Little Endian Integers                           */
                UInt16_LE             = DataType::LittleEndian | DataType::Unsigned16Bit,
                UInt32_LE             = DataType::LittleEndian | DataType::Unsigned32Bit,
                UInt64_LE             = DataType::LittleEndian | DataType::Unsigned64Bit,

                Int16_LE              = DataType::LittleEndian | DataType::Signed16Bit,
                Int32_LE              = DataType::LittleEndian | DataType::Signed32Bit,
                Int64_LE              = DataType::LittleEndian | DataType::Signed64Bit,

                /*                     Big Endian Integers                            */
                UInt16_BE             = DataType::BigEndian | DataType::Unsigned16Bit,
                UInt32_BE             = DataType::BigEndian | DataType::Unsigned32Bit,
                UInt64_BE             = DataType::BigEndian | DataType::Unsigned64Bit,

                Int16_BE              = DataType::BigEndian | DataType::Signed16Bit,
                Int32_BE              = DataType::BigEndian | DataType::Signed32Bit,
                Int64_BE              = DataType::BigEndian | DataType::Signed64Bit,

                /*                 Characters & Strings                               */
                AsciiCharacter        = DataType::Characters | DataType::Ascii,
                UnicodeCharacter      = DataType::Characters | DataType::Unicode,
                Character             = DataType::AsciiCharacter,

                Array                 = DataType::Vector,
                AsciiString           = DataType::Vector | DataType::Characters | DataType::Ascii,
                UnicodeString         = DataType::Vector | DataType::Characters | DataType::Unicode,

                Blob                  = DataType::Undefined,

            };

        public:
            static bool isInteger(DataType::Type type);
            static bool isSigned(DataType::Type type);
            static bool isString(DataType::Type type);
            static bool isAscii(DataType::Type type);
            static bool isUnicode(DataType::Type type);
            static bool isArray(DataType::Type type);
            static bool isLittleEndian(DataType::Type type);
            static bool isBigEndian(DataType::Type type);
            static int sizeOf(DataType::Type type);
            static int byteWidth(DataType::Type type);
            static int bitWidth(DataType::Type type);
            static QSysInfo::Endian byteOrder(DataType::Type type);
            static QString stringValue(DataType::Type type);
    };
}
#endif // PREFSDK_DATATYPE_H
