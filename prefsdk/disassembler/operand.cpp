#include "operand.h"

namespace PrefSDK
{
    Operand::Operand(OperandTypes::Type type, QObject *parent): QObject(parent), _type(type)
    {
    }

    OperandTypes::Type Operand::type() const
    {
        return this->_type;
    }

    const QString &Operand::value() const
    {
        return this->_value;
    }

    void Operand::setDisplayValue(const QString &val)
    {
        this->_value = val;
    }
}
