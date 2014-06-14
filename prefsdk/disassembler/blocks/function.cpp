#include "function.h"

namespace PrefSDK
{    
    Function::Function(FunctionTypes::Type type, const QString &name, uint64_t startaddress, uint64_t endaddress, QObject *parent): Block(startaddress, endaddress, parent), _type(type), _name(name)
    {

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

    const QString &Function::name() const
    {
        return this->_name;
    }

    QString Function::references() const
    {
        if(this->_references.isEmpty())
            return QString();

        QString s = "# XREFS: ";

        for(ReferenceSet::ConstIterator it = this->_references.begin(); it != this->_references.end(); it++)
        {
            if(it != this->_references.begin())
                s.append(" | ");

            s.append(QString("%1").arg((*it)->address(), 8, 16, QLatin1Char('0')).toUpper());
        }

        return s;
    }

    ListingTypes::Type Function::objectType() const
    {
        return ListingTypes::Function;
    }
}
