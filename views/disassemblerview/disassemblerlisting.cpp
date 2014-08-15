#include "disassemblerlisting.h"

DisassemblerListing::DisassemblerListing(QObject *parent): QObject(parent), _currentinstruction(nullptr), _currentaddress(0)
{

}

void DisassemblerListing::addSegment(const QString &name, SegmentTypes::Type segmenttype, uint64_t startaddress, uint64_t endaddress, uint64_t baseoffset)
{
    if(this->_segments.contains(startaddress))
        return;

    this->_addresslist.append(startaddress);
    this->_segments[startaddress] = new Segment(name, segmenttype, startaddress, endaddress, baseoffset);

    std::sort(this->_addresslist.begin(), this->_addresslist.end()); /* Sort Segments by Address */
}

void DisassemblerListing::addEntryPoint(const QString &name, uint64_t address)
{
    this->addFunction(FunctionTypes::EntryPoint, address, name);
    this->_stack.push(address);
}

void DisassemblerListing::addFunction(FunctionTypes::Type type, uint64_t address, const QString &name)
{       
    Segment* segment = this->segmentFromAddress(address);

    if(!segment)
    {
        qDebug() << "ERROR: Segment Not Found"; //TODO: Handle Error Here!
        return;
    }

    if(segment->containsFunction(address))
        return;

    QString funcname = name.isEmpty() ? QString("sub_%1").arg(QString::number(address, 16).toUpper()) : name;
    this->setSymbol(address, DataType::Invalid, funcname);
    segment->addFunction(new Function(type, address, segment));
}

Instruction *DisassemblerListing::addInstruction(uint64_t address)
{
    Segment* segment = this->segmentFromAddress(address);

    if(!segment)
    {
        qDebug() << "ERROR: Segment Not Found"; //TODO: Handle Error Here!
        return nullptr;
    }

    Instruction* instruction = new Instruction(address, (address - segment->startAddress()) + segment->baseOffset(), this->_symboltable);

    this->_currentinstruction = instruction;
    this->_instructions[address] = instruction;
    return instruction;
}

void DisassemblerListing::setSymbol(uint64_t address, DataType::Type type, const QString &name)
{
    this->_symboltable[address] = new Symbol(type, name, this);
}

void DisassemblerListing::addReference(uint64_t srcaddress, uint64_t destaddress, ReferenceTypes::Type referencetype)
{
    if(!this->_references.contains(destaddress))
        this->_references[destaddress] = ReferenceSet();

    ReferenceSet& references = this->_references[destaddress];
    references.insert(new Reference(srcaddress, destaddress, referencetype));
}

Segment *DisassemblerListing::segment(int idx)
{
    return this->_segments[this->_addresslist[idx]];
}

Segment *DisassemblerListing::segmentFromAddress(uint64_t address)
{
    for(int i = 0; i < this->_addresslist.count(); i++)
    {
        Segment* segment = this->_segments[this->_addresslist[i]];

        if(address >= segment->startAddress() && address <= segment->endAddress())
            return segment;
    }

    return nullptr;
}

int DisassemblerListing::segmentsCount() const
{
    return this->_addresslist.count();
}

int DisassemblerListing::functionsCount() const
{
    int c = 0;

    foreach(Segment* segment, this->_segments)
        c += segment->functionsCount();

    return c;
}

Function *DisassemblerListing::function(int idx)
{
    int curridx = 0;

    foreach(Segment* segment, this->_segments)
    {
        if((curridx + segment->functionsCount()) < idx)
        {
            curridx += segment->functionsCount();
            continue;
        }

        return segment->function(idx - curridx);
    }

    return nullptr;
}

DisassemblerListing::ReferenceSet DisassemblerListing::references(uint64_t address) const
{
    if(this->_references.contains(address))
        return this->_references[address];

    return DisassemblerListing::ReferenceSet();
}

const SymbolTable &DisassemblerListing::symbolTable() const
{
    return this->_symboltable;
}

Instruction *DisassemblerListing::mergeInstructions(Instruction *instruction1, Instruction *instruction2, const QString &mnemonic, InstructionCategories::Category category, InstructionTypes::Type type)
{
    Function* func = qobject_cast<Function*>(instruction1->parentObject());
    Instruction* instruction = new Instruction(instruction1->address(), instruction1->offset(), this->_symboltable);

    instruction->setSegmentName(instruction1->segmentName());
    instruction->updateSize(instruction1->size() + instruction2->size());
    instruction->setMnemonic(mnemonic);
    instruction->setCategory(category);
    instruction->setType(type);

    func->removeInstruction(instruction2);
    func->replaceInstruction(instruction1, instruction);

    this->_instructions.remove(instruction1->address());
    this->_instructions.remove(instruction2->address());
    this->_instructions[instruction->address()] = instruction;

    return instruction;
}

bool DisassemblerListing::hasMoreInstructions()
{
    if(this->_stack.isEmpty())
        this->populateFunctions();

    return !this->_stack.isEmpty();
}

uint64_t DisassemblerListing::pop()
{
    do /* Pop until we find an undecoded instruction */
    {
        this->_currentaddress = this->_stack.pop();
    }
    while(this->_instructions.contains(this->_currentaddress) || this->segmentFromAddress(this->_currentaddress) == nullptr);

    return this->_currentaddress;
}

void DisassemblerListing::push(uint64_t address, ReferenceTypes::Type referencetype)
{
    if(Reference::isCall(referencetype) || Reference::isJump(referencetype))
    {
        this->addReference(this->_currentaddress, address, referencetype);

        if(Reference::isJump(referencetype))
            this->setSymbol(address, DataType::Invalid, QString("j_%1").arg(QString::number(address, 16).toUpper()));
    }

    if(!this->_instructions.contains(address)) /* Ignore disassembled addresses */
    {
        if(Reference::isCall(referencetype))
            this->addFunction(FunctionTypes::Function, address, QString());

        this->_stack.push(address);
    }
}

void DisassemblerListing::populateFunctions()
{
    foreach(Segment* segment, this->_segments)
    {
        for(int i = 0; i < segment->functionsCount(); i++)
        {
            Instruction* instruction = nullptr;
            Function* f = segment->function(i);
            uint64_t address = f->startAddress();

            do
            {
                instruction = this->_instructions[address];
                f->addInstruction(instruction);
                address += instruction->size();
            }
            while(instruction && (instruction->type() != InstructionTypes::Stop));
        }
    }
}
