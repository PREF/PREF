#ifndef PREFSDK_BITFIELD_H
#define PREFSDK_BITFIELD_H

#include "fieldelement.h"

namespace PrefSDK
{
    class BitField : public FieldElement
    {
        Q_OBJECT

        public:
            explicit BitField(lua_State *l, int bitstart, int bitend, uint64_t offset, DataType::Type datatype, const QString& name, const QUuid& parentid, ElementPool& elementpool, QHexEditData* hexeditdata, QObject *parent = 0);
            uint64_t mask() const;
            int bitStart() const;
            int bitEnd() const;

        public: /* Overriden Methods */
            virtual ElementType::Type elementType() const;
            virtual uint64_t size() const;
            virtual QString displayType() const;
            virtual QString displayName() const;
            virtual QString displayValue() const;
            virtual bool isOverflowed() const;

        private:
            static uint64_t createMask(int bitstart, int bitend);

        private:
            int _bitstart;
            int _bitend;
            uint64_t _mask;
    };

}

#endif // PREFSDK_BITFIELD_H
