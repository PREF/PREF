#include "function.h"

namespace PrefSDK
{    
    Function::Function(FunctionTypes::Type type, const QString &name, uint64_t startaddress, uint64_t endaddress, QObject *parent): Block(startaddress, endaddress, parent), _type(type), _name(name)
    {

    }

    void Function::addReference(uint64_t address, ReferenceTypes::Type referencetype)
    {
        this->_referencelist.append(address);
        this->_references[address] = referencetype;

        std::sort(this->_referencelist.begin(), this->_referencelist.end());
    }

    void Function::addInstruction(Instruction *instruction)
    {
        instruction->setSegmentName(this->segmentName());
        instruction->setParentObject(this);

        this->_addresslist.append(instruction->address());
        this->_instructions[instruction->address()] = instruction;

        this->_endaddress += instruction->size();
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

    const QString &Function::name() const
    {
        return this->_name;
    }

    QString Function::references() const
    {
        QString refs = "# XREF ";

        for(int i = 0; i < this->_referencelist.count(); i++)
        {
            if (i > 0)
                refs.append(" | ");

            refs.append(QString("%1").arg(this->_referencelist[i], 8, 16, QLatin1Char('0')).toUpper()).append("h");
        }

        return refs;
    }

    ListingTypes::Type Function::objectType() const
    {
        return ListingTypes::Function;
    }
}
