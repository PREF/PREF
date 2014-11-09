#include "segment.h"

namespace PrefSDK
{    
    Segment::Segment(const QString& name, SegmentType::Type type, const DataValue &startaddress, const DataValue &size, const DataValue &baseoffset, QObject *parent): Block(startaddress, size, parent)
    {
        this->_name = name;
        this->_type = type;
        this->_baseoffset = baseoffset;
    }

    const DataValue &Segment::baseOffset() const
    {
        return this->_baseoffset;
    }

    QString Segment::name() const
    {
        return this->_name;
    }

    SegmentType::Type Segment::type() const
    {
        return this->_type;
    }

    Block::Type Segment::blockType() const
    {
        return Block::SegmentBlock;
    }
}
