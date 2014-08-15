#include "function.h"

namespace PrefSDK
{    
    Function::Function(FunctionTypes::Type type, uint64_t startaddress, QObject *parent): Block(startaddress, 0, parent), _type(type)
    {

    }

    void Function::addInstruction(Instruction *instruction)
    {
        if(this->_instructions.contains(instruction->address()))
            return;

        instruction->setSegmentName(this->segmentName());
        instruction->setParentObject(this);

        this->_addresslist.append(instruction->address());
        this->_instructions[instruction->address()] = instruction;

        this->_size += instruction->size();
    }

    void Function::removeInstruction(Instruction *instruction)
    {
        int idx = this->_addresslist.indexOf(instruction->address());

        if(idx == -1)
            return;

        this->_addresslist.removeAt(idx);
        this->_instructions.remove(instruction->address());

        this->_size -= instruction->size();
    }

    void Function::replaceInstruction(Instruction *oldinstruction, Instruction *newinstruction)
    {
        if(oldinstruction->address() != newinstruction->address())
            return;

        this->_instructions[oldinstruction->address()] = newinstruction;

        if(oldinstruction->size() != newinstruction->size())
        {
            this->_size -= oldinstruction->size();
            this->_size += newinstruction->size();
        }
    }

    int Function::indexOf(Instruction *instruction) const
    {
        return this->_addresslist.indexOf(instruction->address());
    }

    Instruction *Function::instruction(int idx)
    {
        return this->_instructions[this->_addresslist[idx]];
    }

    int Function::instructionsCount() const
    {
        return this->_addresslist.count();
    }

    FunctionTypes::Type Function::type() const
    {
        return this->_type;
    }

    bool Function::isEntryPoint() const
    {
        return this->_type & FunctionTypes::EntryPoint;
    }

    bool Function::isImport() const
    {
        return this->_type & FunctionTypes::Import;
    }

    bool Function::isExport() const
    {
        return this->_type & FunctionTypes::Export;
    }

    ListingTypes::Type Function::objectType() const
    {
        return ListingTypes::Function;
    }
}
