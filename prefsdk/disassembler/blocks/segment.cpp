#include "segment.h"

namespace PrefSDK
{    
    Segment::Segment(const QString& name, SegmentTypes::Type type, uint64_t startaddress, uint64_t endaddress, uint64_t baseoffset, QObject *parent): Block(startaddress, endaddress, parent)
    {
        this->_name = name;
        this->_type = type;
        this->_baseoffset = baseoffset;

        this->setSegmentName(name);
    }

    void Segment::addFunction(Function *f)
    {
        f->setSegmentName(this->_name);
        f->setParentObject(this);

        this->_addresslist.append(f->startAddress());
        this->_functions[f->startAddress()] = f;

        if(f->type() == FunctionTypes::EntryPoint)
            this->_entrypoints.append(f);
    }

    int Segment::indexOf(Function *f) const
    {
        return this->_addresslist.indexOf(f->startAddress());
    }

    Function* Segment::function(int idx)
    {
        return this->_functions[this->_addresslist[idx]];
    }

    Function *Segment::entryPoint(int idx)
    {
        return this->_entrypoints[idx];
    }

    int Segment::functionsCount() const
    {
        return this->_addresslist.count();
    }

    int Segment::entryPointsCount() const
    {
        return this->_entrypoints.count();
    }

    uint64_t Segment::baseOffset() const
    {
        return this->_baseoffset;
    }

    QString Segment::name() const
    {
        return this->_name;
    }

    SegmentTypes::Type Segment::type() const
    {
        return this->_type;
    }

    ListingTypes::Type Segment::objectType() const
    {
        return ListingTypes::Segment;
    }
}
