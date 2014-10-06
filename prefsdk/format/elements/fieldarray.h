#ifndef PREFSDK_FIELDARRAY_H
#define PREFSDK_FIELDARRAY_H

#include "fieldelement.h"
#include "field.h"

namespace PrefSDK
{
    class FieldArray : public FieldElement
    {
        Q_OBJECT

        Q_PROPERTY(PrefSDK::DataType::Type itemType READ itemType)
        Q_PROPERTY(quint64 itemCount READ itemCount)

        public:
            FieldArray(DataType::Type itemtype, quint64 itemcount, quint64 offset, const QString& name, const QUuid& parentid, AbstractTree* formattree, QObject *parent = 0);
            DataType::Type itemType() const;
            quint64 itemCount() const;

        public slots:
            PrefSDK::Field* item(int i);
            virtual QString displayName() const;
            virtual QString displayType() const;
            virtual QString displayValue() const;

        public: /* Overriden Methods */
            virtual bool isDynamic() const;
            virtual bool hasChildren() const;
            virtual FormatElement::Type elementType() const;
            virtual quint64 size() const;
            virtual bool isOverflowed() const;
            virtual int indexOf(FormatElement* fe) const;
            virtual void parseChildren();

        protected:
            virtual void pushValue(lua_State* l);
            Q_INVOKABLE int metaIndex(lua_State* l, lua_Integer key);

        private:
            DataType::Type _itemtype;
            quint64 _itemcount;
            QList<QUuid> _items;
    };
}

#endif // PREFSDK_FIELDARRAY_H
