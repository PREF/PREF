#include "symboltable.h"

namespace PrefSDK
{
    SymbolTable::SymbolTable(QObject *parent): QObject(parent)
    {
    }

    lua_Integer SymbolTable::count() const
    {
        return this->_symboltable.count();
    }

    bool SymbolTable::contains(const DataValue &address) const
    {
        return this->_symboltable.contains(address);
    }

    void SymbolTable::set(const DataValue &address, const QString &name)
    {
        this->_symboltable[address] = name;
    }

    QString SymbolTable::get(const DataValue &address) const
    {
        return this->_symboltable[address];
    }
}
