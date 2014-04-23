#include "instruction.h"

namespace PrefSDK
{
    Instruction::Instruction(uint64_t virtualaddress, uint64_t address): _type(0), _datatype(DataType::Invalid), _size(0), _address(address), _virtualaddress(virtualaddress)
    {

    }

    void Instruction::setType(uint64_t type)
    {
        this->_type = type;
    }

    void Instruction::updateSize(uint64_t size)
    {
        this->_size += size;
    }

    uint64_t Instruction::virtualAddress()
    {
        return this->_virtualaddress;
    }

    uint64_t Instruction::address()
    {
        return this->_address;
    }

    uint64_t Instruction::instructionType()
    {
        return this->_type;
    }

    uint64_t Instruction::instructionSize()
    {
        return this->_size;
    }

    Operand* Instruction::operand1()
    {
        return &this->_operands.Op1;
    }

    Operand* Instruction::operand2()
    {
        return &this->_operands.Op2;
    }

    Operand* Instruction::operand3()
    {
        return &this->_operands.Op3;
    }

    Operand* Instruction::operand4()
    {
        return &this->_operands.Op4;
    }

    Operand* Instruction::operand5()
    {
        return &this->_operands.Op5;
    }

    Operand* Instruction::operand6()
    {
        return &this->_operands.Op6;
    }
}
