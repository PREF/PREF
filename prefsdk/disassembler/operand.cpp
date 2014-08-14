#include "operand.h"

namespace PrefSDK
{
    Operand::Operand(OperandTypes::Type type, DataType::Type datatype, QObject *parent): QObject(parent), _operandtype(type), _datatype(datatype)
    {
    }

    Operand::Operand(const Operand *operand, QObject *parent): QObject(parent), _operandtype(operand->_operandtype), _datatype(operand->_datatype), _operandvalue(operand->_operandvalue), _displayvalue(operand->_displayvalue), _registername(operand->_registername)
    {

    }

    OperandTypes::Type Operand::type() const
    {
        return this->_operandtype;
    }

    DataType::Type Operand::dataType() const
    {
        return this->_datatype;
    }

    int8_t Operand::valueInt8() const
    {
        return this->_operandvalue.value<int8_t>();
    }

    int16_t Operand::valueInt16() const
    {
        return this->_operandvalue.value<int16_t>();
    }

    int32_t Operand::valueInt32() const
    {
        return this->_operandvalue.value<int32_t>();
    }

    int64_t Operand::valueInt64() const
    {
        return this->_operandvalue.value<int64_t>();
    }

    uint8_t Operand::valueUInt8() const
    {
        return this->_operandvalue.value<uint8_t>();
    }

    uint16_t Operand::valueUInt16() const
    {
        return this->_operandvalue.value<uint16_t>();
    }

    uint32_t Operand::valueUInt32() const
    {
        return this->_operandvalue.value<uint32_t>();
    }

    uint64_t Operand::valueUInt64() const
    {
        return this->_operandvalue.value<uint64_t>();
    }

    const QString &Operand::registerName() const
    {
        return this->_registername;
    }

    QString Operand::displayValue() const
    {
        if(!this->_displayvalue.isEmpty())
            return this->_displayvalue;
        else if(this->_operandtype == OperandTypes::Register)
            return QString("$%1").arg(this->_registername);

        int width = DataType::byteWidth(this->_datatype);

        if(DataType::isSigned(this->_datatype))
        {
            switch(DataType::bitWidth(this->_datatype))
            {
                case 8:
                    return QString("%1").arg(this->_operandvalue.value<int8_t>(), width, 16, QLatin1Char('0')).toUpper().append("h");

                case 16:
                    return QString("%1").arg(this->_operandvalue.value<int16_t>(), width, 16, QLatin1Char('0')).toUpper().append("h");

                case 32:
                    return QString("%1").arg(this->_operandvalue.value<int32_t>(), width, 16, QLatin1Char('0')).toUpper().append("h");

                case 64:
                    return QString("%1").arg(this->_operandvalue.value<int64_t>(), width, 16, QLatin1Char('0')).toUpper().append("h");

                default:
                    break;
            }
        }
        else
        {
            switch(DataType::bitWidth(this->_datatype))
            {
                case 8:
                    return QString("%1").arg(this->_operandvalue.value<uint8_t>(), width, 16, QLatin1Char('0')).toUpper().append("h");

                case 16:
                    return QString("%1").arg(this->_operandvalue.value<uint16_t>(), width, 16, QLatin1Char('0')).toUpper().append("h");

                case 32:
                    return QString("%1").arg(this->_operandvalue.value<uint32_t>(), width, 16, QLatin1Char('0')).toUpper().append("h");

                case 64:
                    return QString("%1").arg(this->_operandvalue.value<uint64_t>(), width, 16, QLatin1Char('0')).toUpper().append("h");

                default:
                    break;
            }
        }

        return this->_operandvalue.toString();
    }

    void Operand::setValueInt8(int8_t value)
    {
        this->_operandvalue = value;
    }

    void Operand::setValueInt16(int16_t value)
    {
        this->_operandvalue = value;
    }

    void Operand::setValueInt32(int32_t value)
    {
        this->_operandvalue = value;
    }

    void Operand::setValueInt64(int64_t value)
    {
        this->_operandvalue = static_cast<qint64>(value);
    }

    void Operand::setValueUInt8(uint8_t value)
    {
        this->_operandvalue = value;
    }

    void Operand::setValueUInt16(uint16_t value)
    {
        this->_operandvalue = value;
    }

    void Operand::setValueUInt32(uint32_t value)
    {
        this->_operandvalue = value;
    }

    void Operand::setValueUInt64(uint64_t value)
    {
        this->_operandvalue = static_cast<quint64>(value);
    }

    void Operand::setRegisterName(const QString &regname)
    {
        this->_registername = regname;
    }

    void Operand::setDisplayValue(const QString &displvalue)
    {
        this->_displayvalue = displvalue;
    }
}
