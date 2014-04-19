#ifndef PREFSDK_STRUCTURE_H
#define PREFSDK_STRUCTURE_H

#include "formatelement.h"
#include "fieldarray.h"
#include "field.h"

namespace PrefSDK
{
    class Structure : public FormatElement
    {
        Q_OBJECT

        private:
            typedef QHash<uint64_t, QUuid> FieldMap;
            typedef QList<uint64_t> FieldOffsetList;

        public:
            explicit Structure(lua_State *l, uint64_t offset, const QString& name, const QUuid& parentid, ElementPool& elementpool, QHexEditData* hexeditdata, QObject *parent = 0);
            Structure* addStructure(const QString& name);
            FieldElement* addField(DataType::Type datatype, const QString& name, uint64_t count = 1);
            const FormatElement* field(int i) const;
            uint64_t fieldCount() const;


        public: /* Overriden Methods */
            virtual bool hasChildren() const;
            virtual bool isDynamic() const;
            virtual ElementType::Type elementType() const;
            virtual uint64_t size() const;
            virtual int indexOf(FormatElement* fe) const;
            virtual QString displayType() const;
            virtual QString displayValue() const;

        private:
            FieldMap _fieldids;
            FieldOffsetList _fieldoffsets;
    };

}

#endif // PREFSDK_STRUCTURE_H
