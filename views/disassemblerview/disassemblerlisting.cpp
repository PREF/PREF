#include "disassemblerlisting.h"

DisassemblerListing::DisassemblerListing(QObject *parent): QObject(parent)
{

}

void DisassemblerListing::addSegment(Segment *segment)
{
    this->_addresslist.append(segment->startAddress());
    this->_segments[segment->startAddress()] = segment;
}

Segment *DisassemblerListing::segment(int idx)
{
    return this->_segments[this->_addresslist[idx]];
}

int DisassemblerListing::segmentsCount() const
{
    return this->_addresslist.count();
}
