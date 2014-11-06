#ifndef PREFSDK_DATATYPE_H
#define PREFSDK_DATATYPE_H

#include <QObject>

namespace PrefSDK
{
    class DataType: public QObject
    {
        Q_OBJECT

        Q_ENUMS(Type)

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
            explicit DataType(QObject* parent = 0);

        public:
            static bool isInteger(DataType::Type type);
            static bool isSigned(DataType::Type type);
            static bool isString(DataType::Type type);
            static bool isAscii(DataType::Type type);
            static bool isUnicode(DataType::Type type);
            static bool isArray(DataType::Type type);
            static bool isLittleEndian(DataType::Type type);
            static bool isBigEndian(DataType::Type type);
            static uint sizeOf(DataType::Type type);
            static uint byteWidth(DataType::Type type);
            static uint bitWidth(DataType::Type type);
            static QSysInfo::Endian byteOrder(DataType::Type type);
            static DataType::Type adjust(DataType::Type type, QSysInfo::Endian byteorder);
            static QString stringValue(DataType::Type type);
    };

    /*
    template<DataType::Type DATATYPE> struct TypeOf { };
    template<> struct TypeOf<DataType::Int8> { typedef qint8 Type; };
    template<> struct TypeOf<DataType::UInt8> { typedef quint8 Type; };
    template<> struct TypeOf<DataType::Int16> { typedef qint16 Type; };
    template<> struct TypeOf<DataType::UInt16> { typedef quint16 Type; };
    template<> struct TypeOf<DataType::Int32> { typedef qint32 Type; };
    template<> struct TypeOf<DataType::UInt32> { typedef quint32 Type; };
    template<> struct TypeOf<DataType::Int64> { typedef qint64 Type; };
    template<> struct TypeOf<DataType::UInt64> { typedef quint64 Type; };

    template<int WIDTH> struct WidthOf { };
    template<> struct WidthOf<8> { constexpr DataType::Type SignedDataType = DataType::Int8; constexpr DataType::Type UnsignedDataType = DataType::UInt8; };
    template<> struct WidthOf<16> { constexpr DataType::Type SignedDataType = DataType::Int16; constexpr DataType::Type UnsignedDataType = DataType::UInt16; };
    template<> struct WidthOf<32> { constexpr DataType::Type SignedDataType = DataType::Int32; constexpr DataType::Type UnsignedDataType = DataType::UInt32; };
    template<> struct WidthOf<64> { constexpr DataType::Type SignedDataType = DataType::Int64; constexpr DataType::Type UnsignedDataType = DataType::UInt64; };
    */
}
#endif // PREFSDK_DATATYPE_H
