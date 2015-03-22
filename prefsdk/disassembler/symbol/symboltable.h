#ifndef PREFSDK_SYMBOLTABLE_H
#define PREFSDK_SYMBOLTABLE_H

#include "symbol.h"
#include "prefsdk/type/datavalue.h"
#include <QObject>
#include <QHash>
#include <QMap>
#include <lua.hpp>

namespace PrefSDK
{
    class SymbolTable : public QObject
    {
        Q_OBJECT

        public:
            typedef typename QHash<DataValue, Symbol*>::Iterator Iterator;

        public:
            explicit SymbolTable(DataType::Type addresstype, QObject *parent = 0);
            lua_Integer count() const;
            DataType::Type addressType() const;
            SymbolTable::Iterator begin();
            SymbolTable::Iterator end();
            bool contains(const DataValue& address) const;
            bool isType(const DataValue& address, Symbol::Type symboltype) const;
            void set(Symbol::Type symboltype, const DataValue& address, const QString &symbolname);
            void set(Symbol::Type symboltype, const DataValue& address, const DataValue &calleraddress, const QString& name);
            void set(Symbol::Type symboltype, const DataValue& address, const DataValue &size, const DataValue &calleraddress);
            void set(Symbol::Type symboltype, const DataValue& address, const DataValue &size, const DataValue &calleraddress, const QString& name);
            QString name(const DataValue& address) const;
            Symbol* get(const DataValue& address) const;
            QList<Symbol*> functions() const;
            QList<Symbol*> labels() const;
            QList<Symbol*> variables() const;
            QList<Symbol*> strings() const;

        public:
            Q_INVOKABLE bool contains(lua_Integer address);
            Q_INVOKABLE void set(lua_Integer address, lua_Integer size, lua_Integer calleraddress, lua_Integer symboltype, const QString& name);
            Q_INVOKABLE void set(lua_Integer address, lua_Integer calleraddress, lua_Integer symboltype, const QString& name);
            Q_INVOKABLE void set(lua_Integer address, lua_Integer size, lua_Integer calleraddress, lua_Integer symboltype);
            Q_INVOKABLE void set(lua_Integer address, lua_Integer calleraddress, lua_Integer symboltype);
            Q_INVOKABLE void set(lua_Integer address, lua_Integer symboltype, const QString& name);
            Q_INVOKABLE void set(lua_Integer address, lua_Integer symboltype);
            Q_INVOKABLE QString name(lua_Integer address);
            Q_INVOKABLE lua_Integer type(lua_Integer address);

        private:
            void popSymbol(Symbol* symbol);
            void pushSymbol(Symbol* symbol);

        private:
            DataType::Type _addresstype;
            QHash<DataValue, Symbol*> _symboltable;
            QMap<DataValue, Symbol*> _functions;
            QMap<DataValue, Symbol*> _labels;
            QMap<DataValue, Symbol*> _variables;
            QMap<DataValue, Symbol*> _strings;
    };
}

#endif // PREFSDK_SYMBOLTABLE_H
