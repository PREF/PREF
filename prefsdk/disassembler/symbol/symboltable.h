#ifndef PREFSDK_SYMBOLTABLE_H
#define PREFSDK_SYMBOLTABLE_H

#include "prefsdk/type/datavalue.h"
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
            void set(const DataValue& address, const QString& name);
            QString get(const DataValue& address) const;

        private:
            QHash<DataValue, QString> _symboltable;
    };
}

#endif // PREFSDK_SYMBOLTABLE_H
