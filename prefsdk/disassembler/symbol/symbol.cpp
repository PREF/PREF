#include "symbol.h"

namespace PrefSDK
{
    Symbol::Symbol(Symbol::Type symboltype, const DataValue &address, const DataValue &size, DataType::Type datatype, const QString &name, QObject *parent): QObject(parent), _symboltype(symboltype), _address(address), _size(size), _datatype(datatype), _name(name)
    {

    }

    const PrefSDK::DataValue &Symbol::address() const
    {
        return this->_address;
    }

    const PrefSDK::DataValue &Symbol::size() const
    {
        return this->_size;
    }

    PrefSDK::DataType::Type Symbol::dataType() const
    {
        return this->_datatype;
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
        this->_size = size;
    }

    void Symbol::setDataType(PrefSDK::DataType::Type datatype)
    {
        this->_datatype = datatype;
    }

    void Symbol::setName(const QString &name)
    {
        this->_name = name;
    }

    void Symbol::setType(const PrefSDK::Symbol::Type symboltype)
    {
        this->_symboltype = symboltype;
    }
}
