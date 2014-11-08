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
            explicit Symbol(Symbol::Type symboltype, const DataValue& address, const DataValue& size, const QString& name, QObject *parent = 0);
            PrefSDK::Symbol::Type type() const;
            const QString& name() const;
            void setSize(const PrefSDK::DataValue& size);
            void setName(const QString& name);
            void setType(const PrefSDK::Symbol::Type symboltype);

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;

        private:
            PrefSDK::Symbol::Type _symboltype;
            QString _name;
    };
}

#endif // PREFSDK_SYMBOL_H
