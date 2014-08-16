#include "function.h"

namespace PrefSDK
{    
    Function::Function(FunctionTypes::Type type, uint64_t startaddress, QObject *parent): Block(startaddress, 0, parent), _type(type)
    {

    }

    void Function::addInstruction(Instruction *instruction)
    {
        instruction->setSegmentName(this->segmentName());
        instruction->setParentObject(this);

        this->_instructions.append(instruction);
        this->_size += instruction->size();

        std::sort(this->_instructions.begin(), this->_instructions.end(), &Function::sortInstructions); /* Sort Instructions by Address */
    }

    void Function::removeInstruction(Instruction *instruction)
    {
        int idx = this->_instructions.indexOf(instruction);

        if(idx == -1)
            return;

        this->_instructions.removeAt(idx);
        this->_size -= instruction->size();
    }

    void Function::replaceInstruction(Instruction *oldinstruction, Instruction *newinstruction)
    {
        if(oldinstruction->address() != newinstruction->address()) /* Check if the addresses are the same! */
            return;

        int idx = this->_instructions.indexOf(oldinstruction);

        if(idx == -1)
            return;

        this->_instructions[idx] = newinstruction;

        if(oldinstruction->size() != newinstruction->size())
        {
            this->_size -= oldinstruction->size();
            this->_size += newinstruction->size();
        }
    }

    int Function::indexOf(Instruction *instruction) const
    {
        return this->_instructions.indexOf(instruction);
    }

    Instruction *Function::instruction(int idx)
    {
        return this->_instructions[idx];
    }

    int Function::instructionsCount() const
    {
        return this->_instructions.count();
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

    bool Function::sortInstructions(Instruction *instruction1, Instruction *instruction2)
    {
        return instruction1->startAddress() < instruction2->startAddress();
    }
}
