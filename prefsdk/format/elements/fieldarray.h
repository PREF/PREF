#ifndef FIELDARRAY_H
#define FIELDARRAY_H

#include <QtCore>
#include <QtWidgets>
#include "prefsdk/format/elements/field.h"

namespace PrefSDK
{
    class FieldArray : public FieldElement
    {
        Q_OBJECT

        public:
            typedef QList<lua_Integer> OffsetList;
            typedef QHash<lua_Integer, Field*> FieldMap;

        public:
            explicit FieldArray(lua_State* l, DataType::Type itemtype, lua_Integer offset, QString name, quint64 itemcount, ByteBuffer* bytebuffer, LuaCTable* model, FormatElement* formatobject, DataType::Type type = DataType::List, QObject *parent = 0);
            virtual void setBase(int base);
            DataType::Type itemDataType() const;
            int elementCount() const;
            Field* element(int i) const;
            int indexOf(Field *f) const;

        public: /* Overriden Methods */
            virtual FormatElement::FormatObjectType objectType();
            virtual QString displayType();
            virtual QString displayName();
            virtual QString displayValue();
            virtual lua_Integer value();
            virtual lua_Integer size();

        protected:
            virtual void metaIndex(lua_State* l);

        private:
            DataType::Type _itemtype;
            quint64 _elementcount;
            OffsetList _offsetlist;
            FieldMap _fieldmap;
    };
}
#endif // FIELDARRAY_H
