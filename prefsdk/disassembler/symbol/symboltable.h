#ifndef PREFSDK_SYMBOLTABLE_H
#define PREFSDK_SYMBOLTABLE_H

#include "symbol.h"
#include <QObject>
#include <QHash>
#include <lua.hpp>

namespace PrefSDK
{
    class SymbolTable : public QObject
    {
        Q_OBJECT

        public:
            explicit SymbolTable(QObject *parent = 0);
            lua_Integer count() const;
            bool contains(const DataValue& address) const;
            bool isType(const DataValue& address, Symbol::Type symboltype) const;
            void set(Symbol::Type symboltype, const DataValue& address, const QString& name);
            void set(Symbol::Type symboltype, const DataValue& address, DataType::Type datatype, const QString& name);
            QString name(const DataValue& address) const;
            Symbol* get(const DataValue& address) const;

        private:
            QHash<DataValue, Symbol*> _symboltable;
    };
}

#endif // PREFSDK_SYMBOLTABLE_H
