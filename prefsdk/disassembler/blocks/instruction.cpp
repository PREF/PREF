#include "instruction.h"

namespace PrefSDK
{
    const QString Instruction::INVALID_MNEMONIC = "???";

    Instruction::Instruction(const DataValue &address, const DataValue &offset, DataType::Type opcodetype, QHexEditData *hexeditdata, QObject *parent): Block(address, parent), _hexeditdata(hexeditdata), _mnemonic(Instruction::INVALID_MNEMONIC), _opcodetype(opcodetype), _offset(offset), _category(InstructionCategory::Undefined), _type(InstructionType::Undefined)
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

    void Instruction::cloneOperand(QObject *op)
    {
        this->_operands.append(new Operand(qobject_cast<Operand*>(op)));
    }

    PrefSDK::Operand* Instruction::addOperand(lua_Integer operandtype, lua_Integer datatype)
    {
        Operand* operand = new Operand(operandtype, datatype);

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
