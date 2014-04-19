#ifndef PREFSDK_FIELD_H
#define PREFSDK_FIELD_H

#include "prefsdk/numericlimits.h"
#include "fieldelement.h"
#include "bitfield.h"

namespace PrefSDK
{
    class Field : public FieldElement
    {
        Q_OBJECT

        public:
            explicit Field(lua_State *l, DataType::Type datatype, uint64_t offset, const QString& name, const QUuid& parentid, ElementPool& elementpool, QHexEditData* hexeditdata, QObject *parent = 0);
            uint bitFieldCount() const;
            const BitField *bitField(int64_t i) const;
            const BitField *bitField(const QString &name) const;
            BitField* setBitField(const QString& name, int bitstart, int bitend);

        public: /* Overriden Methods */
            virtual bool isDynamic() const;
            virtual ElementType::Type elementType() const;
            virtual int indexOf(FormatElement* fe) const;
            virtual QString displayValue() const;
            virtual bool isOverflowed() const;

        private:
            QHash<QString, QUuid> _bitfieldids;
    };
}

#endif // PREFSDK_FIELD_H
