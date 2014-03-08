#include "disassemblerrange.h"

namespace PrefSDK
{
    DisassemblerRange::DisassemblerRange()
    {
        this->_start = this->_end = 0;
    }

    DisassemblerRange::DisassemblerRange(lua_Integer start, lua_Integer end)
    {
        this->_start = start;
        this->_end = end;
    }

    DisassemblerRange::DisassemblerRange(const DisassemblerRange &dr)
    {
        *this = dr;
    }

    lua_Integer DisassemblerRange::start() const
    {
        return this->_start;
    }

    lua_Integer DisassemblerRange::end() const
    {
        return this->_end;
    }

    bool DisassemblerRange::contains(lua_Integer addr) const
    {
        return (addr >= this->_start && addr <= this->_end);
    }

    bool DisassemblerRange::isEmpty() const
    {
        return this->_start >= this->_end;
    }

    lua_Integer DisassemblerRange::size() const
    {
        return this->_end - this->_start;
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
