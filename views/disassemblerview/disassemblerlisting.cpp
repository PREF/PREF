#include "disassemblerlisting.h"

DisassemblerListing::DisassemblerListing(QObject *parent): QObject(parent), _currentinstruction(nullptr), _currentaddress(0)
{

}

void DisassemblerListing::addSegment(const QString &name, SegmentTypes::Type segmenttype, uint64_t startaddress, uint64_t endaddress, uint64_t baseoffset)
{
    this->_segments.append(new Segment(name, segmenttype, startaddress, endaddress, baseoffset));
    std::sort(this->_segments.begin(), this->_segments.end(), &DisassemblerListing::sortBlocks); /* Sort Segments by Address */
}

void DisassemblerListing::addEntryPoint(const QString &name, uint64_t address)
{
    if(this->addFunction(FunctionTypes::EntryPoint, address, name))
        this->_stack.push(address);
}

bool DisassemblerListing::addFunction(FunctionTypes::Type type, uint64_t address, const QString &name)
{       
    Segment* segment = this->segmentFromAddress(address);

    if(!segment)
    {
        qDebug() << "ERROR: Segment Not Found"; //TODO: Handle Error Here!
        return false;
    }

    if(segment->containsFunction(address))
        return false;

    Function* func = new Function(type, address, segment);
    segment->addFunction(func);
    this->_functions.append(func);

    this->setSymbol(address, DataType::Invalid, (name.isEmpty() ? QString("sub_%1").arg(QString::number(address, 16).toUpper()) : name));
    std::sort(this->_functions.begin(), this->_functions.end(), &DisassemblerListing::sortBlocks);
    return true;
}

Instruction *DisassemblerListing::createInstruction(uint64_t address)
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

Instruction *DisassemblerListing::instructionFromAddress(uint64_t address)
{
    if(!this->_instructions.contains(address))
        return nullptr;

    return this->_instructions[address];
}

Instruction *DisassemblerListing::nextInstruction(Instruction *instruction)
{
    uint64_t nextaddress = instruction->address() + instruction->size();

    if(!this->_instructions.contains(nextaddress))
        return nullptr;

    return this->_instructions[nextaddress];
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
    if((idx < 0) || (idx >= this->_segments.count()))
        return nullptr;

    return this->_segments[idx];
}

Segment *DisassemblerListing::segmentFromAddress(uint64_t address)
{
    foreach(Segment* segment, this->_segments)
    {
        if(address >= segment->startAddress() && address <= segment->endAddress())
            return segment;
    }

    return nullptr;
}

int DisassemblerListing::segmentsCount() const
{
    return this->_segments.count();
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
    if((idx < 0) || (idx >= this->_functions.count()))
        return nullptr;

    return this->_functions[idx];
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
    Instruction* instruction = new Instruction(instruction1->address(), instruction1->offset(), this->_symboltable);

    instruction->setSegmentName(instruction1->segmentName());
    instruction->updateSize(instruction1->size() + instruction2->size());
    instruction->setMnemonic(mnemonic);
    instruction->setCategory(category);
    instruction->setType(type);

    this->_instructions.remove(instruction1->address());
    this->_instructions.remove(instruction2->address());
    this->_instructions[instruction->address()] = instruction;
    return instruction;
}

bool DisassemblerListing::hasNextInstruction(Instruction *instruction)
{
    return this->_instructions.contains(instruction->address() + instruction->size());
}

bool DisassemblerListing::hasMoreInstructions()
{
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

bool DisassemblerListing::sortBlocks(Block *block1, Block *block2)
{
    return block1->startAddress() < block2->startAddress();
}
