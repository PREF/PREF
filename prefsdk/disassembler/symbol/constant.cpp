#include "constant.h"

namespace PrefSDK
{
    Constant::Constant(const QString &name, const DataValue &value, QObject *parent): QObject(parent), _name(name), _value(value)
    {
    }

    const QString &Constant::name() const
    {
        return this->_name;
    }

    const DataValue &Constant::value() const
    {
        return this->_value;
    }
}
