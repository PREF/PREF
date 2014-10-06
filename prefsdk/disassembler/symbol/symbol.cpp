#include "symbol.h"

namespace PrefSDK
{
    Symbol::Symbol(DataType::Type datatype, const QString &name, QObject *parent): QObject(parent), _datatype(datatype), _name(name)
    {

    }

    DataType::Type Symbol::dataType() const
    {
        return this->_datatype;
    }

    const QString &Symbol::name() const
    {
        return this->_name;
    }
}
