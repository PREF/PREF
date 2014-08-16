#include "segment.h"

namespace PrefSDK
{    
    Segment::Segment(const QString& name, SegmentTypes::Type type, uint64_t startaddress, uint64_t size, uint64_t baseoffset, QObject *parent): Block(startaddress, size, parent)
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

        if(f->type() == FunctionTypes::EntryPoint)
            this->_entrypoints.append(f);

        this->_functions.append(f);
        std::sort(this->_functions.begin(), this->_functions.end(), &Segment::sortFunctions); /* Sort Functions by Address */
    }

    bool Segment::containsFunction(uint64_t address)
    {
        foreach(Function* func, this->_functions)
        {
            if(func->contains(address))
                return true;
        }

        return false;
    }

    int Segment::indexOf(Function *f) const
    {
        return this->_functions.indexOf(f);
    }

    Function* Segment::function(int idx)
    {
        return this->_functions[idx];
    }

    Function *Segment::entryPoint(int idx)
    {
        return this->_entrypoints[idx];
    }

    int Segment::functionsCount() const
    {
        return this->_functions.count();
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

    bool Segment::sortFunctions(Function *function1, Function *function2)
    {
        return function1->startAddress() < function2->startAddress();
    }
}
