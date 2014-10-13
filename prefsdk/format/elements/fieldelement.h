#ifndef PREFSDK_FIELDELEMENT_H
#define PREFSDK_FIELDELEMENT_H

#include "prefsdk/type/datatype.h"
#include "formatelement.h"

namespace PrefSDK
{
    class FieldElement : public FormatElement
    {
        Q_OBJECT

        Q_PROPERTY(PrefSDK::DataType::Type dataType READ dataType)
        Q_PROPERTY(bool issigned READ isSigned)
        Q_PROPERTY(bool isinteger READ isInteger)
        Q_PROPERTY(bool isarray READ isArray)

        public:
            explicit FieldElement(DataType::Type datatype, quint64 offset, const QString& name, const QUuid& parentid, AbstractTree* formattree, QObject *parent = 0);
            DataType::Type dataType() const;
            bool isSigned() const;
            bool isInteger() const;
            bool isArray() const;

        public: /* Overriden Methods */
            virtual int indexOf(FormatElement*) const;
            virtual quint64 size() const;

        public slots:
            virtual QString displayType() const;
            virtual QString displayValue() const;

        public: /* Abstract Methods */
            virtual bool isOverflowed() const = 0;

        protected:
            DataType::Type _datatype;
    };
}

#endif // PREFSDK_FIELDELEMENT_H
