#ifndef PREFSDK_FIELDARRAY_H
#define PREFSDK_FIELDARRAY_H

#include "fieldelement.h"
#include "field.h"

namespace PrefSDK
{
    class FieldArray : public FieldElement
    {
        Q_OBJECT

        public:
            FieldArray(lua_State *l, DataType::Type itemtype, uint64_t itemcount, uint64_t offset, const QString& name, const QUuid& parentid, ElementPool& elementpool, QHexEditData* hexeditdata, QObject *parent = 0);
            const Field* item(int i) const;
            DataType::Type itemType() const;
            uint64_t itemCount() const;

        public: /* Overriden Methods */
            virtual bool isDynamic() const;
            virtual bool hasChildren() const;
            virtual ElementType::Type elementType() const;
            virtual QString displayType() const;
            virtual QString displayName() const;
            virtual QString displayValue() const;
            virtual uint64_t size() const;
            virtual bool isOverflowed() const;
            virtual int indexOf(FormatElement* fe) const;
            virtual void parseChildren();

        private:
            DataType::Type _itemtype;
            uint64_t _itemcount;
            QList<QUuid> _items;
    };
}

#endif // PREFSDK_FIELDARRAY_H
