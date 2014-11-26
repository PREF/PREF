#ifndef PREFSDK_FIELD_H
#define PREFSDK_FIELD_H

#include "prefsdk/libs/qt/qtlua.h"
#include "prefsdk/numericlimits.h"
#include "fieldelement.h"
#include "bitfield.h"

namespace PrefSDK
{
    class Field : public FieldElement
    {
        Q_OBJECT

        public:
            explicit Field(DataType::Type datatype, quint64 offset, const QString& name, const QUuid& parentid, AbstractTree* formattree, QObject *parent = 0);
            uint bitFieldCount() const;
            const BitField *bitField(qint64 i) const;
            const BitField *bitField(const QString &name) const;

        public slots:
            PrefSDK::BitField* setBitField(const QString& name, lua_Integer bitstart, lua_Integer bitend);
            PrefSDK::BitField* setBitField(const QString& name, lua_Integer bitstart);

        protected:
            virtual void pushValue(lua_State* l);
            Q_INVOKABLE virtual int metaIndex(lua_State* l, const QString& key);

        public: /* Overriden Methods */
            virtual bool isDynamic() const;
            virtual bool hasChildren() const;
            virtual FormatElement::Type elementType() const;
            virtual int indexOf(FormatElement* fe) const;
            virtual bool isOverflowed() const;
            virtual void setBase(int b);

        public slots:
            virtual QString displayValue() const;

        private:
            QHash<QString, QUuid> _bitfieldids;
    };
}

#endif // PREFSDK_FIELD_H
