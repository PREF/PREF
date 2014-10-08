#include "symbol.h"

namespace PrefSDK
{
    Symbol::Symbol(Symbol::Type symboltype, const DataValue &address, DataType::Type datatype, const QString &name, QObject *parent): QObject(parent), _symboltype(symboltype), _address(address), _datatype(datatype), _name(name)
    {

    }

    const DataValue &Symbol::address() const
    {
        return this->_address;
    }

    DataType::Type Symbol::dataType() const
    {
        return this->_datatype;
    }

    Symbol::Type Symbol::type() const
    {
        return this->_symboltype;
    }

    const QString &Symbol::name() const
    {
        return this->_name;
    }

    void Symbol::setDataType(DataType::Type datatype)
    {
        this->_datatype = datatype;
    }

    void Symbol::setName(const QString &name)
    {
        this->_name = name;
    }

    void Symbol::setType(const Symbol::Type symboltype)
    {
        this->_symboltype = symboltype;
    }
}
