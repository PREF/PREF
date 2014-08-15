#include "instruction.h"

namespace PrefSDK
{
    Instruction::Instruction(uint64_t address, uint64_t offset, const SymbolTable &symboltable, QObject *parent): Block(address, 0, parent), _symboltable(symboltable), _category(InstructionCategories::Undefined), _type(InstructionTypes::Undefined), _mnemonic("???"), _opcode(0xFFFFFFFF), _address(address), _offset(offset)
    {

    }

    bool Instruction::contains(uint64_t address)
    {
        return (address >= this->_address) && (address < (this->_address + this->_size));
    }

    void Instruction::clearOperands()
    {
        this->_operands.clear();
    }

    void Instruction::cloneOperand(Operand *operand)
    {
        this->_operands.append(new Operand(operand));
    }

    Operand* Instruction::addOperand(OperandTypes::Type operandtype, DataType::Type datatype)
    {
        Operand* operand = new Operand(operandtype, datatype);

        this->_operands.append(operand);
        return operand;
    }

    void Instruction::removeOperand(int idx)
    {
        if(idx >= this->_operands.count())
            return;

        this->_operands.removeAt(idx);
    }

    void Instruction::updateSize(uint64_t sz)
    {
        this->_size += sz;
    }

    void Instruction::setFormat(const QString &s)
    {
        this->_opformat = s;
    }

    InstructionCategories::Category Instruction::category() const
    {
        return this->_category;
    }

    InstructionTypes::Type Instruction::type() const
    {
        return this->_type;
    }

    uint64_t Instruction::address() const
    {
        return this->_address;
    }

    uint64_t Instruction::offset() const
    {
        return this->_offset;
    }

    uint64_t Instruction::size() const
    {
        return this->_size;
    }

    uint64_t Instruction::opCode() const
    {
        return this->_opcode;
    }

    QString Instruction::mnemonic() const
    {
        return this->_mnemonic;
    }

    QString Instruction::displayHexDump(QHexEditData* hexeditdata) const
    {
        QString hexdump;
        QHexEditDataReader reader(hexeditdata);
        QByteArray ba = reader.read(this->_offset, this->_size);

        for(int i = 0; i < ba.length(); i++)
        {
            if(!hexdump.isEmpty())
                hexdump.append(" ");

            hexdump.append(QString("%1").arg(static_cast<unsigned char>(ba.at(i)), 2, 16, QLatin1Char('0')).toUpper());
        }

        return hexdump;
    }

    QString Instruction::displayOperands() const
    {
        if(this->_opformat.isEmpty())
            return this->standardOperandFormat();

        return this->customOperandformat();
    }

    int Instruction::operandsCount() const
    {
        return this->_operands.count();
    }

    Operand *Instruction::operand(int idx) const
    {
        return this->_operands[idx];
    }

    void Instruction::setOpCode(uint64_t opcode)
    {
        this->_opcode = opcode;
    }

    void Instruction::setMnemonic(const QString &mnemonic)
    {
        this->_mnemonic = mnemonic;
    }

    void Instruction::setCategory(InstructionCategories::Category category)
    {
        this->_category = category;
    }

    void Instruction::setType(InstructionTypes::Type type)
    {
        this->_type = type;
    }

    ListingTypes::Type Instruction::objectType() const
    {
        return ListingTypes::Instruction;
    }

    QString Instruction::displayAddress() const
    {
        return QString("%1").arg(this->_address, 8, 16, QLatin1Char('0')).toUpper();
    }

    QString Instruction::standardOperandFormat() const
    {
        QString s;

        for(int i = 0; i < this->_operands.count(); i++)
        {
            Operand* operand = this->_operands[i];

            if(i > 0)
                s.append(", ");

            if((operand->type() == OperandTypes::Address) && this->_symboltable.contains(operand->valueUInt64()))
                s.append(this->_symboltable[operand->valueUInt64()]->name());
            else
                s.append(operand->displayValue());
        }

        return s;
    }

    QString Instruction::customOperandformat() const
    {
        int i = 0;
        QChar ch;
        QString s;

        while(i < this->_opformat.length())
        {
            ch = this->_opformat[i];

            switch(ch.toLatin1())
            {
                case '%':
                {
                    ch = this->_opformat[++i];

                    if(ch.isNull())
                        break;

                    if(ch == '%')
                    {
                        s.append("%");
                        break;
                    }

                    bool ok = false;
                    int opidx = QString(ch).toInt(&ok);

                    if(!ok || (opidx < 0 || opidx > this->_operands.count())) //TODO: Handle Errors
                    {
                        if(!ok)
                            qDebug() << "ERROR: Invalid Operand Format: Expected Integer not '" << ch << "'";
                        else
                            qDebug() << "ERROR: Operand Index out of range";

                        continue;
                    }

                    Operand* operand = this->_operands[opidx - 1];

                    if((operand->type() == OperandTypes::Address) && this->_symboltable.contains(operand->valueUInt64()))
                        s.append(this->_symboltable[operand->valueUInt64()]->name());
                    else
                        s.append(operand->displayValue());

                    break;
                }

                case '\0':
                    return s; /* Something wrong: reached EOS */

                default:
                {
                    s.append(ch);
                    break;
                }
            }

            i++;
        }

        return s;
    }
}
