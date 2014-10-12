#include "operand.h"

namespace PrefSDK
{
    Operand::Operand(lua_Integer type, lua_Integer descriptor, lua_Integer datatype, QObject *parent): QObject(parent), _operandtype(type), _operanddescriptor(descriptor), _datatype(datatype)
    {

    }

    Operand::Operand(const Operand *operand, QObject *parent): QObject(parent), _operandtype(operand->_operandtype), _operanddescriptor(operand->_operanddescriptor), _datatype(operand->_datatype), _operandvalue(operand->_operandvalue), _registername(operand->_registername)
    {

    }

    lua_Integer Operand::type() const
    {
        return this->_operandtype;
    }

    lua_Integer Operand::descriptor() const
    {
        return this->_operanddescriptor;
    }

    lua_Integer Operand::dataType() const
    {
        return this->_datatype;
    }

    lua_Integer Operand::value() const
    {
        return this->_operandvalue.compatibleValue<lua_Integer>();
    }

    void Operand::setValue(lua_Integer v)
    {
        this->_operandvalue = DataValue::create(v, static_cast<DataType::Type>(this->_datatype));
    }

    void Operand::setDataType(lua_Integer dt)
    {
        this->_datatype = static_cast<DataType::Type>(dt);
    }

    const DataValue &Operand::operandValue() const
    {
        return this->_operandvalue;
    }

    const QString &Operand::registerName() const
    {
        return this->_registername;
    }

    void Operand::setType(lua_Integer t)
    {
        this->_operandtype = t;
    }

    void Operand::setDescriptor(lua_Integer d)
    {
        this->_operanddescriptor = d;
    }

    void Operand::setRegisterName(const QString &regname)
    {
        this->_registername = regname;
    }
}
