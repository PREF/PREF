#include "disassemblerlisting.h"

DisassemblerListing::DisassemblerListing(QObject *parent): QObject(parent)
{

}

void DisassemblerListing::addSegment(Segment *segment)
{
    this->_addresslist.append(segment->startAddress());
    this->_segments[segment->startAddress()] = segment;
}

void DisassemblerListing::addReference(uint64_t srcaddress, uint64_t destaddress, ReferenceTypes::Type referencetype)
{
    if(!this->_references.contains(destaddress))
        this->_references[destaddress] = ReferenceSet();

    ReferenceSet& references = this->_references[destaddress];
    references.insert(new Reference(srcaddress, referencetype));
}

Segment *DisassemblerListing::segment(int idx)
{
    return this->_segments[this->_addresslist[idx]];
}

int DisassemblerListing::segmentsCount() const
{
    return this->_addresslist.count();
}

DisassemblerListing::ReferenceSet DisassemblerListing::references(uint64_t address) const
{
    if(this->_references.contains(address))
        return this->_references[address];

    return DisassemblerListing::ReferenceSet();
}
