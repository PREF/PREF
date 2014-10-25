#ifndef PREFSDK_REFERENCE_H
#define PREFSDK_REFERENCE_H

#include "prefsdk/type/datavalue.h"
#include <QObject>

namespace PrefSDK
{
    class Reference : public QObject
    {
        Q_OBJECT

        Q_ENUMS(Type)

        public:
            enum Type
            {
                Flow            = 0x00000000,
                Code            = 0x10000000,
                Data            = 0x20000000,
                EntryPoint      = 0x40000000,

                Unconditional   = 0x01000000,
                Conditional     = 0x02000000,

                Call            = Code | Unconditional | 0x00000001,
                ConditionalCall = Code | Conditional   | 0x00000002,

                Jump            = Code | Unconditional | 0x00000004,
                ConditionalJump = Code | Conditional   | 0x00000008,

                Address         = Data | 0x00000001,
                Read            = Data | 0x00000002,
                Write           = Data | 0x00000004,

                Invalid         = 0xFFFFFFFF,
            };

        public:
            explicit Reference(const DataValue& referencedby, Reference::Type referencetype, QObject *parent = 0);
            const DataValue& referencedBy() const;
            Reference::Type type() const;

        public:
            static bool isCodeReference(Reference::Type referencetype);
            static bool isDataReference(Reference::Type referencetype);
            static bool isCall(Reference::Type referencetype);
            static bool isJump(Reference::Type referencetype);
            static bool isConditional(Reference::Type referencetype);            

        private:
            DataValue _referencedby;
            Reference::Type _referencetype;
    };

}

#endif // PREFSDK_REFERENCE_H
