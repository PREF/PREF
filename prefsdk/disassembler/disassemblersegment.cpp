#include "disassemblersegment.h"

namespace PrefSDK
{    
    DisassemblerSegment::DisassemblerSegment(): DisassemblerRange()
    {

    }

    lua_Integer DisassemblerSegment::baseAddress() const
    {
        return this->_baseaddress;
    }

    DisassemblerSegment::DisassemblerSegment(lua_Integer baseaddress, lua_Integer start, lua_Integer end, QString name, SegmentType type): DisassemblerRange(start, end)
    {
        this->_baseaddress = baseaddress;
        this->_name = name;
        this->_type = type;
    }

    QString DisassemblerSegment::name() const
    {
        return this->_name;
    }

    DisassemblerSegment::SegmentType DisassemblerSegment::type() const
    {
        return this->_type;
    }
}
