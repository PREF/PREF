#include "instruction.h"

namespace PrefSDK
{
    const QString Instruction::INVALID_MNEMONIC = "???";

    Instruction::Instruction(const DataValue &address, const DataValue &offset, DataType::Type opcodetype, QHexEditData *hexeditdata, QObject *parent): Block(address, parent), _source(-1), _destination(-1), _base(-1), _displacement(-1), _scale(-1), _hexeditdata(hexeditdata), _mnemonic(Instruction::INVALID_MNEMONIC), _opcodetype(opcodetype), _offset(offset), _category(InstructionCategory::Undefined), _type(InstructionType::Undefined)
    {

    }

    bool Instruction::isJump() const
    {
        return (this->_type == InstructionType::Jump) || (this->_type == InstructionType::ConditionalJump);
    }

    bool Instruction::isCall() const
    {
        return (this->_type == InstructionType::Call) || (this->_type == InstructionType::ConditionalCall);
    }

    bool Instruction::isValid() const
    {
        return (QString::compare(this->_mnemonic, Instruction::INVALID_MNEMONIC) != 0);
    }

    void Instruction::clearOperands()
    {
        this->_operands.clear();
    }

    void Instruction::resetFormat()
    {
        this->_format = QString();
    }

    void Instruction::checkDescriptor(Operand::Descriptor operanddescriptor)
    {
        switch(operanddescriptor)
        {
            case Operand::Source:
                this->_source = this->_operands.length();
                break;

            case Operand::Destination:
                this->_destination = this->_operands.length();
                break;

            case Operand::Base:
                this->_base = this->_operands.length();
                break;

            case Operand::Displacement:
                this->_displacement = this->_operands.length();
                break;

            case Operand::Scale:
                this->_scale = this->_operands.length();
                break;

            default:
                break;
        }
    }

    void Instruction::cloneOperand(QObject *op)
    {
        this->_operands.append(new Operand(qobject_cast<Operand*>(op)));
    }

    Operand *Instruction::addOperand(lua_Integer operandtype, lua_Integer datatype)
    {
        return this->addOperand(operandtype, Operand::Nothing, datatype);
    }

    PrefSDK::Operand* Instruction::addOperand(lua_Integer operandtype, lua_Integer operanddescriptor, lua_Integer datatype)
    {
        this->checkDescriptor(static_cast<Operand::Descriptor>(operanddescriptor));

        Operand* operand = new Operand(operandtype, operanddescriptor, datatype);
        this->_operands.append(operand);
        return operand;
    }

    void Instruction::removeOperand(lua_Integer idx)
    {
        if(idx >= this->_operands.count())
            return;

        this->_operands.removeAt(idx);
    }

    void Instruction::setFormat(const QString &s)
    {
        this->_format = s;
    }

    Block::Type Instruction::blockType() const
    {
        return Block::InstructionBlock;
    }

    lua_Integer Instruction::category() const
    {
        return this->_category;
    }

    lua_Integer Instruction::type() const
    {
        return this->_type;
    }

    lua_Integer Instruction::address() const
    {
        return this->_startaddress.compatibleValue<lua_Integer>();
    }

    lua_Integer Instruction::offset() const
    {
        return this->_offset.compatibleValue<lua_Integer>();
    }

    lua_Integer Instruction::size() const
    {
        return this->_size.compatibleValue<lua_Integer>();
    }

    lua_Integer Instruction::opcode() const
    {
        return this->_opcode.compatibleValue<lua_Integer>();
    }

    lua_Integer Instruction::source() const
    {
        return this->_source;
    }

    lua_Integer Instruction::destination() const
    {
        return this->_destination;
    }

    lua_Integer Instruction::base() const
    {
        return this->_base;
    }

    lua_Integer Instruction::displacement() const
    {
        return this->_displacement;
    }

    lua_Integer Instruction::scale() const
    {
        return this->_scale;
    }

    const QString &Instruction::format() const
    {
        return this->_format;
    }

    const QString &Instruction::mnemonic() const
    {
        return this->_mnemonic;
    }

    PrefSDK::Operand *Instruction::firstOperand() const
    {
        return this->_operands.first();
    }

    PrefSDK::Operand *Instruction::lastOperand() const
    {
        return this->_operands.last();
    }

    Operand *Instruction::sourceOperand() const
    {
        if(this->_source == -1)
            return nullptr;

        return this->_operands[this->_source];
    }

    Operand *Instruction::destinationOperand() const
    {
        if(this->_destination == -1)
            return nullptr;

        return this->_operands[this->_destination];
    }

    Operand *Instruction::baseOperand() const
    {
        if(this->_base == -1)
            return nullptr;

        return this->_operands[this->_base];
    }

    Operand *Instruction::displacementOperand() const
    {
        if(this->_destination == -1)
            return nullptr;

        return this->_operands[this->_destination];
    }

    Operand *Instruction::scaleOperand() const
    {
        if(this->_scale == -1)
            return nullptr;

        return this->_operands[this->_scale];
    }

    QString Instruction::hexDump() const
    {
        QString hexdump;
        QHexEditDataReader reader(this->_hexeditdata);
        QByteArray ba = reader.read(this->_offset.compatibleValue<qint64>(), this->_size.compatibleValue<qint64>());

        for(int i = 0; i < ba.length(); i++)
        {
            if(!hexdump.isEmpty())
                hexdump.append(" ");

            hexdump.append(QString("%1").arg(static_cast<unsigned char>(ba.at(i)), 2, 16, QLatin1Char('0')).toUpper());
        }

        return hexdump;
    }

    lua_Integer Instruction::operandsCount() const
    {
        return this->_operands.count();
    }

    PrefSDK::Operand *Instruction::operand(lua_Integer idx) const
    {
        if((idx < 0) || idx >= this->_operands.count())
        {
            throw PrefException(QString("Trying to get Operand %1 from Instruction at %2h (Mnemonic: '%3')").arg(QString::number(idx), this->_startaddress.toString(16), this->_mnemonic));
            return nullptr;
        }

        return this->_operands[idx];
    }

    const DataValue &Instruction::opcodeValue() const
    {
        return this->_opcode;
    }

    void Instruction::setOpcode(lua_Integer opc)
    {
        this->_opcode = DataValue::create(opc, this->_opcodetype);
    }

    void Instruction::setSource(lua_Integer idx)
    {
        this->_source = idx;
    }

    void Instruction::setDestination(lua_Integer idx)
    {
        this->_destination = idx;
    }

    void Instruction::setBase(lua_Integer idx)
    {
        this->_base = idx;
    }

    void Instruction::setDisplacement(lua_Integer idx)
    {
        this->_displacement = idx;
    }

    void Instruction::setScale(lua_Integer idx)
    {
        this->_scale = idx;
    }

    void Instruction::setSize(const DataValue &size)
    {
        this->_size = size;
    }

    void Instruction::setMnemonic(const QString &mnemonic)
    {
        this->_mnemonic = mnemonic;
    }

    const DataValue &Instruction::offsetValue() const
    {
        return this->_offset;
    }

    void Instruction::setCategory(lua_Integer category)
    {
        this->_category = category;
    }

    void Instruction::setType(lua_Integer type)
    {
        this->_type = type;
    }

    lua_Integer Instruction::next(lua_Integer datatype)
    {
        DataType::Type dt = static_cast<DataType::Type>(datatype);
        this->_size += DataValue::create(DataType::sizeOf(dt), dt);

        LuaHexEditData luahexeditdata(this->_hexeditdata);
        return luahexeditdata.readType(this->offset(), datatype);
    }
}
