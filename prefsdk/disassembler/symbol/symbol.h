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
            enum Type { Unknown, Library, Function, Jump, Address, Immediate, String };

        public:
            explicit Symbol(Symbol::Type symboltype, const DataValue& address, const DataValue& size, DataType::Type datatype, const QString& name, QObject *parent = 0);
            const PrefSDK::DataValue& address() const;
            const PrefSDK::DataValue& size() const;
            PrefSDK::DataType::Type dataType() const;
            PrefSDK::Symbol::Type type() const;
            const QString& name() const;
            void setSize(const PrefSDK::DataValue& size);
            void setDataType(PrefSDK::DataType::Type datatype);
            void setName(const QString& name);
            void setType(const PrefSDK::Symbol::Type symboltype);

        private:
            PrefSDK::Symbol::Type _symboltype;
            PrefSDK::DataValue _address;
            PrefSDK::DataValue _size;
            PrefSDK::DataType::Type _datatype;
            QString _name;
    };
}

#endif // PREFSDK_SYMBOL_H
