#include "symbol.h"

namespace PrefSDK
{
    Symbol::Symbol(Symbol::Type symboltype, const DataValue &address, const DataValue &size, const QString &name, QObject *parent): Block(address, size, parent), _symboltype(symboltype), _name(name)
    {

    }

    PrefSDK::Symbol::Type Symbol::type() const
    {
        return this->_symboltype;
    }

    const QString &Symbol::name() const
    {
        return this->_name;
    }

    void Symbol::setSize(const PrefSDK::DataValue &size)
    {
        this->setEndAddress(this->_startaddress + size);
    }

    void Symbol::setName(const QString &name)
    {
        this->_name = name;
    }

    void Symbol::setType(const PrefSDK::Symbol::Type symboltype)
    {
        this->_symboltype = symboltype;
    }

    Block::Type Symbol::blockType() const
    {
        return Block::SymbolBlock;
    }
}
