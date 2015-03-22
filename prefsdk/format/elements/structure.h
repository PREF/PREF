#ifndef PREFSDK_STRUCTURE_H
#define PREFSDK_STRUCTURE_H

#include "prefsdk/libs/qt/qtlua.h"
#include "formatelement.h"
#include "fieldarray.h"
#include "field.h"

namespace PrefSDK
{
    class Structure : public FormatElement
    {
        Q_OBJECT

        Q_PROPERTY(qint64 fieldCount READ fieldCount)

        private:
            typedef QHash<qint64, QUuid> FieldMap;     /* Offset -> UUID */
            typedef QHash<QString, qint64> FieldNames; /* Name -> Offset */
            typedef QList<qint64> FieldOffsets;

        public:
            explicit Structure(quint64 offset, const QString& name, const QUuid& parentid, AbstractTree* formattree, QObject *parent = 0);
            qint64 fieldCount() const;

        protected:
            Q_INVOKABLE virtual int metaIndex(lua_State* l, const QString& key);

        public slots:
            PrefSDK::Structure* addStructure(const QString& name);
            PrefSDK::FieldElement* addField(lua_Integer datatype, const QString& name, lua_Integer count = 1);
            PrefSDK::FormatElement* field(lua_Integer i) const;
            PrefSDK::FormatElement* field(const QString& s) const;

        public: /* Overriden Methods */
            virtual bool hasChildren() const;
            virtual bool isDynamic() const;
            virtual FormatElement::Type elementType() const;
            virtual quint64 size() const;
            virtual int indexOf(FormatElement* fe) const;
            virtual void setBase(int b);

        public slots:
            virtual QString displayType() const;
            virtual QString displayValue() const;

        private:
            FieldMap _fieldids;
            FieldNames _fieldnames;
            FieldOffsets _fieldoffsets;
    };
}

#endif // PREFSDK_STRUCTURE_H
