#include "instruction.h"

namespace PrefSDK
{
    Instruction::Instruction(uint64_t address, uint64_t offset, QObject *parent): ListingObject(parent), _category(InstructionCategories::Undefined), _type(InstructionTypes::Undefined), _address(address), _offset(offset), _size(0)
    {

    }

    void Instruction::addOperand(Operand *operand)
    {
        this->_operands.append(operand);
    }

    void Instruction::setSize(uint64_t sz)
    {
        this->_size = sz;
    }

    void Instruction::formatInstruction(const QString &s)
    {
        this->_displayoperand = s;
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
        return this->_displayoperand;
    }

    int Instruction::operandsCount() const
    {
        return this->_operands.count();
    }

    Operand *Instruction::operand(int idx) const
    {
        return this->_operands[idx];
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
}
