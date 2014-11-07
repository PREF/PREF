#ifndef PREFSDK_SYMBOL_H
#define PREFSDK_SYMBOL_H

#include "prefsdk/type/datatype.h"
#include "prefsdk/type/datavalue.h"
#include "prefsdk/disassembler/blocks/block.h"
#include <QObject>

namespace PrefSDK
{
    class Symbol : public Block
    {
        Q_OBJECT

        Q_ENUMS(Type)

        public:
            enum Type { Unknown, Address, Label, String, Function, Library };

        public:
            explicit Symbol(Symbol::Type symboltype, const DataValue& address, const DataValue& size, DataType::Type datatype, const QString& name, QObject *parent = 0);
            PrefSDK::DataType::Type dataType() const;
            PrefSDK::Symbol::Type type() const;
            const QString& name() const;
            void setSize(const PrefSDK::DataValue& size);
            void setDataType(PrefSDK::DataType::Type datatype);
            void setName(const QString& name);
            void setType(const PrefSDK::Symbol::Type symboltype);

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;

        private:
            PrefSDK::Symbol::Type _symboltype;
            PrefSDK::DataType::Type _datatype;
            QString _name;
    };
}

#endif // PREFSDK_SYMBOL_H
