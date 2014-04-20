#include "disassemblerrange.h"

namespace PrefSDK
{
    DisassemblerRange::DisassemblerRange()
    {
        this->_start = this->_end = 0;
    }

    DisassemblerRange::DisassemblerRange(quint64 start, quint64 end)
    {
        this->_start = start;
        this->_end = end;
    }

    DisassemblerRange::DisassemblerRange(const DisassemblerRange &dr)
    {
        *this = dr;
    }

    quint64 DisassemblerRange::start() const
    {
        return this->_start;
    }

    quint64 DisassemblerRange::end() const
    {
        return this->_end;
    }

    quint64 DisassemblerRange::size() const
    {
        return this->_end - this->_start;
    }

    bool DisassemblerRange::contains(quint64 addr) const
    {
        return (addr >= this->_start && addr <= this->_end);
    }

    bool DisassemblerRange::isEmpty() const
    {
        return this->_start >= this->_end;
    }

    DisassemblerRange &DisassemblerRange::operator=(const DisassemblerRange &rhs)
    {
        if(this != &rhs)
        {
            this->_start = rhs._start;
            this->_end = rhs._end;
        }

        return *this;
    }
}
