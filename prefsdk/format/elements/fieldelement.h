#ifndef PREFSDK_FIELDELEMENT_H
#define PREFSDK_FIELDELEMENT_H

#include "prefsdk/datatype.h"
#include "formatelement.h"

namespace PrefSDK
{
    class FieldElement : public FormatElement
    {
        Q_OBJECT

        public:
            explicit FieldElement(lua_State *l, DataType::Type datatype, uint64_t offset, const QString& name, const QUuid& parentid, ElementPool& elementpool, QHexEditData* hexeditdata, QObject *parent = 0);
            DataType::Type dataType() const;
            bool isSigned() const;
            bool isInteger() const;

        public: /* Overriden Methods */
            virtual int indexOf(FormatElement*) const;
            virtual uint64_t size() const;
            virtual QString displayType() const;
            virtual QString displayValue() const;

        public: /* Abstract Methods */
            virtual bool isOverflowed() const = 0;

        protected:
            DataType::Type _datatype;
    };
}

#endif // PREFSDK_FIELDELEMENT_H
