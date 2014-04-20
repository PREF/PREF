#include "operand.h"

namespace PrefSDK
{
    Operand::Operand()
    {
        this->_optype = Operand::Void;
    }

    Operand::Type Operand::operandType()
    {
        return this->_optype;
    }

    DataType::Type Operand::dataType()
    {
        return this->_datatype;
    }

    uint64_t Operand::address()
    {
        return this->_address;
    }

    uint64_t Operand::reg()
    {
        return this->_reg;
    }

    uint64_t Operand::phrase()
    {
        return this->_phrase;
    }
}
