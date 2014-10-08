#ifndef PREFSDK_SYMBOL_H
#define PREFSDK_SYMBOL_H

#include "prefsdk/type/datatype.h"
#include "prefsdk/type/datavalue.h"
#include <QObject>

namespace PrefSDK
{
    class Symbol : public QObject
    {
        Q_OBJECT

        Q_ENUMS(Type)

        public:
            enum Type { Unknown, Function, Jump, Address, Immediate, String };

        public:
            explicit Symbol(Symbol::Type symboltype, const DataValue& address, DataType::Type datatype, const QString& name, QObject *parent = 0);
            const DataValue& address() const;
            DataType::Type dataType() const;
            Symbol::Type type() const;
            const QString& name() const;
            void setDataType(DataType::Type datatype);
            void setName(const QString& name);
            void setType(const Symbol::Type symboltype);

        private:
            Symbol::Type _symboltype;
            DataValue _address;
            DataType::Type _datatype;
            QString _name;
    };
}

#endif // PREFSDK_SYMBOL_H
