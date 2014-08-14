#ifndef PREFSDK_REFERENCE_H
#define PREFSDK_REFERENCE_H

#include <QtCore>

namespace PrefSDK
{
    namespace ReferenceTypes
    {
        enum Type
        {
            Flow            = 0x00000000,
            Code            = 0x10000000,
            Data            = 0x20000000,

            Unconditional   = 0x01000000,
            Conditional     = 0x02000000,

            Call            = Code | Unconditional | 0x00000001,
            ConditionalCall = Code | Conditional   | 0x00000002,

            Jump            = Code | Unconditional | 0x00000004,
            ConditionalJump = Code | Conditional   | 0x00000008,

            Address         = Data | 0x00000001,
            Read            = Data | 0x00000002,
            Write           = Data | 0x00000004,
        };
    }

    class Reference : public QObject
    {
        Q_OBJECT

        public:
            explicit Reference(uint64_t srcaddress, uint64_t destaddress, ReferenceTypes::Type referencetype, QObject *parent = 0);
            uint64_t srcAddress() const;
            uint64_t destAddress() const;
            ReferenceTypes::Type type() const;
            QString displayAddress() const;

        public:
            static bool isCodeReference(ReferenceTypes::Type referencetype);
            static bool isDataReference(ReferenceTypes::Type referencetype);
            static bool isCall(ReferenceTypes::Type referencetype);
            static bool isJump(ReferenceTypes::Type referencetype);
            static bool isConditional(ReferenceTypes::Type referencetype);

        private:
            uint64_t _srcaddress;
            uint64_t _destaddress;
            ReferenceTypes::Type _referencetype;
    };

}

#endif // PREFSDK_REFERENCE_H
