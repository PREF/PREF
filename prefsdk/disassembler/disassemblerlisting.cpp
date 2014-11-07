#include "disassemblerlisting.h"

namespace PrefSDK
{
    DisassemblerListing::DisassemblerListing(QHexEditData *hexeditdata, QObject *parent): QObject(parent), _blocksorted(false), _hexeditdata(hexeditdata)
    {
        this->_referencetable = new ReferenceTable(this);
        this->_symboltable = new SymbolTable(this);
        this->_constanttable = new ConstantTable(this);
    }

    void DisassemblerListing::calcFunctionBounds()
    {
        QList<DataValue> funcep = this->_functions.keys();

        for(int i = 0; i < funcep.length(); i++)
        {
            if(i == (funcep.length() - 1))
            {
                this->_functions.last()->setEndAddress(this->_blocks.last()->endAddress());
                return;
            }

            qint64 idx = this->indexOf(funcep[i + 1], Block::InstructionBlock) - 1;
            Block* b = this->_blocks[idx];

            while(b->blockType() != Block::InstructionBlock)
                b = this->_blocks[--idx];

            this->_functions[funcep[i]]->setEndAddress(b->endAddress());
        }
    }

    bool DisassemblerListing::isAddress(const DataValue &address) const
    {
        for(SegmentMap::ConstIterator it = this->_segments.cbegin(); it != this->_segments.cend(); it++)
        {
            Segment* segment = it.value();

            if(segment->contains(address))
                return true;
        }

        return false;
    }

    bool DisassemblerListing::isDecoded(const DataValue& address) const
    {        
        return this->findInstruction(address) != nullptr;
    }

    qint64 DisassemblerListing::length() const
    {
        return this->_blocks.length();
    }

    QHexEditData *DisassemblerListing::data()
    {
        return this->_hexeditdata;
    }

    const DisassemblerListing::BlockList &DisassemblerListing::blocks()
    {
        this->checkSort();
        return this->_blocks;
    }

    ReferenceTable *DisassemblerListing::referenceTable()
    {
        return this->_referencetable;
    }

    void DisassemblerListing::createReference(const DataValue& address, const DataValue &referencedby, Reference::Type referencetype, qint64 insertidx)
    {
        bool newreference = !this->_referencetable->isReferenced(address);
        ReferenceSet* refset = this->_referencetable->addReference(address, referencedby, referencetype);

        if(newreference && Reference::isJump(referencetype)) /* Insert Jump Label in listing */
        {
            if(insertidx == -1)
                this->_blocks.append(refset);
            else
                this->_blocks.insert(insertidx, refset);
        }
    }

    void DisassemblerListing::createLabel(const DataValue &destaddress, Instruction *instruction, const QString &name)
    {
        Segment* segment = this->findSegment(destaddress);

        if(!segment)
            throw PrefException(QString("No segment for: %1").arg(destaddress.toString(16)));

        Label* label = nullptr;

        if(!this->_labels.contains(destaddress))
        {
            label = new Label(destaddress, name, this);

            this->_labels[destaddress] = label;
            this->_symboltable->set(Symbol::Label, destaddress, name);
            this->_blocks.append(label);
        }
        else
            label = this->_labels[destaddress];

        label->addSource(instruction->startAddress());
    }

    void DisassemblerListing::createSegment(const QString &name, Segment::Type segmenttype, const DataValue& startaddress, const DataValue& size, const DataValue& baseoffset)
    {
        if(this->_segments.contains(startaddress))
            return;

        Segment* s = new Segment(name, segmenttype, startaddress, size, baseoffset, this);

        this->_segments[startaddress] = s;
        this->_blocks.append(s);
    }

    void DisassemblerListing::createFunction(FunctionType::Type functiontype, const DataValue &startaddress)
    {
        this->createFunction(QString(), functiontype, startaddress);
    }

    void DisassemblerListing::createFunction(const QString &name, FunctionType::Type functiontype, const DataValue &startaddress)
    {
        if(this->_functions.contains(startaddress))
            return;

        QString funcname = name.isEmpty() ? QString("sub_%1").arg(startaddress.toString(16)) : name;
        Function* f = new Function(functiontype, startaddress, this);
        this->_functions[startaddress] = f;

        if(functiontype == FunctionType::EntryPointFunction)
        {
            this->_entrypoints.append(f);
            std::sort(this->_entrypoints.begin(), this->_entrypoints.end(), &DisassemblerListing::sortBlocks);
        }

        if(!this->_symboltable->contains(startaddress)) /* Apply Symbol, if needed */
            this->_symboltable->set(Symbol::Function, startaddress, funcname);

        this->_blocks.append(f);
    }

    void DisassemblerListing::addInstruction(Instruction* instruction)
    {
        DataValue address = instruction->startAddress();

        if(this->_instructions.contains(address))
        {
            instruction->deleteLater();
            return;
        }

        Segment* segment = this->findSegment(address);

        if(!segment)
        {
            instruction->deleteLater();
            throw PrefException(QString("No segment for: %1").arg(address.toString(16)));
        }

        this->_instructions[address] = instruction;
        this->_blocks.append(instruction);
    }

    Segment *DisassemblerListing::findSegment(Block *block)
    {
        return this->findSegment(block->startAddress());
    }

    Segment *DisassemblerListing::findSegment(const DataValue &address) const
    {
        if(this->_segments.contains(address))  /* We're lucky, return the block directly! */
            return this->_segments[address];

        for(SegmentMap::ConstIterator it = this->_segments.begin(); it != this->_segments.end(); it++)
        {
            Segment* s = it.value();

            if(s->contains(address))
                return s;
        }

        return nullptr;
    }

    Function *DisassemblerListing::findFunction(Block *block)
    {
        return this->findFunction(block->startAddress());
    }

    Function *DisassemblerListing::findFunction(const DataValue &address)
    {
        if(this->_functions.contains(address)) /* We're lucky, return the block directly! */
            return this->_functions[address];

        for(FunctionMap::iterator it = this->_functions.begin(); it != this->_functions.end(); it++)
        {
            Function* f = it.value();

            if(f->contains(address))
                return f;
        }

        return nullptr;
    }

    Instruction *DisassemblerListing::findInstruction(const DataValue &address) const
    {
        if(this->_instructions.contains(address))
            return this->_instructions[address];

        /* Try to find the instruction containing this address */
        InstructionMap::ConstIterator it = this->_instructions.lowerBound(address);

        if(it == this->_instructions.end())
            return nullptr;

        Instruction* i = it.value();

        if((it != this->_instructions.begin()) && (address < i->startAddress()))
        {
            it--;

            while(it != this->_instructions.begin())
            {
                i = it.value();

                if(i->endAddress() < address)
                    break;

                if(i->contains(address))
                    return i;

                it--;
            }
        }

        return nullptr;
    }

    Block *DisassemblerListing::findBlock(const DataValue &address)
    {
        Instruction* instruction = this->findInstruction(address); /* Try With Instructions */

        if(instruction)
            return instruction;

        Function* function = this->findFunction(address); /* Try With Functions */

        if(function)
            return function;

        return nullptr;
    }

    bool DisassemblerListing::hasNextBlock(QObject *b)
    {
        Block* block = qobject_cast<Block*>(b);
        return block != this->_blocks.last();
    }

    QObject* DisassemblerListing::nextBlock(QObject* b)
    {
        Block* block = qobject_cast<Block*>(b);

        if(block == this->_blocks.last())
            return nullptr;

        qint64 idx = this->indexOf(block);

        if(idx == -1)
        {
            throw PrefException("DisassemblerListing::nextBlock(): Cannot find selected block");
            return nullptr;
        }

        return this->_blocks[idx + 1];
    }

    QObject *DisassemblerListing::nextFunction(QObject *f)
    {
        Function* func = qobject_cast<Function*>(f);

        if(func == this->_functions.last())
            return nullptr;

        FunctionMap::Iterator it = this->_functions.find(func->startAddress());

        if(it == this->_functions.end())
            return nullptr;

        return (++it).value();
    }

    QObject *DisassemblerListing::firstInstruction(QObject *f)
    {
        Block* b = qobject_cast<Block*>(f);

        if((b->blockType() != Block::FunctionBlock) || !this->_instructions.contains(b->startAddress()))
            return nullptr;

        return this->_instructions[b->startAddress()];
    }

    QObject *DisassemblerListing::nextInstruction(QObject *i)
    {
        Block* b = qobject_cast<Block*>(i);

        if((b->blockType() != Block::InstructionBlock) || !this->_instructions.contains(b->startAddress()) || !this->_instructions.contains(b->startAddress() + b->sizeValue()))
            return nullptr;

        return this->_instructions[b->startAddress() + b->sizeValue()];
    }

    PrefSDK::Block *DisassemblerListing::firstBlock()
    {
        this->checkSort();
        return this->_blocks.first();
    }

    PrefSDK::Block *DisassemblerListing::lastBlock()
    {
        this->checkSort();
        return this->_blocks.last();
    }

    Function *DisassemblerListing::firstFunction()
    {
        return this->_functions.first();
    }

    Function *DisassemblerListing::lastFunction()
    {
        return this->_functions.last();
    }

    qint64 DisassemblerListing::indexOf(Block *block)
    {
        return this->indexOf(block->startAddress(), block->blockType());
    }

    qint64 DisassemblerListing::indexOf(const DataValue &address, Block::Type blocktype)
    {
        qint64 start = 0, end = this->_blocks.length() - 1, mid = -1;

        this->checkSort();

        while(start <= end)
        {
            mid = (start + end) / 2;
            Block* foundblock = this->_blocks[mid];

            if(foundblock->startAddress() > address)
                end = mid - 1;
            else if(foundblock->startAddress() < address)
                start = mid + 1;
            else
                break;
        }

        if(mid != -1)
        {
            int i = mid;
            Block* b = this->_blocks[mid];

            if(b->blockType() == blocktype)
                return i;

            i = mid - 1;
            b = this->_blocks[i];

            while((i >= 0) && (b->startAddress() == address)) // Look backward
            {
                b = this->_blocks[i];

                if(b->blockType() == blocktype)
                    return i;

                i--;
            }

            i = mid + 1;
            b = this->_blocks[i];

            while((i < this->_blocks.length()) && (b->startAddress() == address)) // Look forward
            {
                b = this->_blocks[i];

                if(b->blockType() == blocktype)
                    return i;

                i++;
            }
        }

        return -1;
    }

    const DisassemblerListing::SegmentMap &DisassemblerListing::segments() const
    {
        return this->_segments;
    }

    const DisassemblerListing::EntryPointList &DisassemblerListing::entryPoints() const
    {
        return this->_entrypoints;
    }

    const DisassemblerListing::InstructionMap &DisassemblerListing::instructions() const
    {
        return this->_instructions;
    }

    const DisassemblerListing::StringSymbolSet &DisassemblerListing::strings() const
    {
        return this->_stringsymbols;
    }

    const DisassemblerListing::VariableSet &DisassemblerListing::variables() const
    {
        return this->_variables;
    }

    const DisassemblerListing::FunctionMap &DisassemblerListing::functions() const
    {
        return this->_functions;
    }

    SymbolTable *DisassemblerListing::symbolTable()
    {
        return this->_symboltable;
    }

    ConstantTable *DisassemblerListing::constantTable()
    {
        return this->_constanttable;
    }

    void DisassemblerListing::checkSort()
    {        
        if(!this->_blocksorted)
        {
            std::sort(this->_blocks.begin(), this->_blocks.end(), &DisassemblerListing::sortBlocks);
            this->_blocksorted = true;
        }
    }

    qint64 DisassemblerListing::pointsToString(const DataValue &address) const
    {
        Segment* segment = this->findSegment(address);

        if(!segment || this->_segments.contains(address) || this->_functions.contains(address) || this->_instructions.contains(address)) /* Ignore code blocks */
            return false;

        DataValue offset = (address - segment->startAddress()) + segment->baseOffset();
        QHexEditDataReader reader(this->_hexeditdata);
        QString s = reader.readString(offset.compatibleValue<qint64>());

        if(s.length() < 4)
            return 0;

        return s.length();
    }

    void DisassemblerListing::removeInstructions(Instruction *from, Instruction *to)
    {
        qint64 startidx = this->indexOf(from), endidx = this->indexOf(to), idx = startidx;

        for(qint64 i = startidx; i <= endidx; i++)
        {
            Block* block = this->_blocks[idx];

            if(block->blockType() != Block::InstructionBlock) /* Leave non-instruction blocks in listing */
            {
                idx++;
                continue;
            }

            this->_instructions.remove(block->startAddress());
            this->_blocks.removeAt(idx);
        }
    }

    bool DisassemblerListing::sortBlocks(Block *block1, Block *block2)
    {
        if(block1->startAddress() == block2->startAddress())
        {
            /* Segments first of All: (Segment < Function | Instruction) */
            if(block1->blockType() == Block::SegmentBlock)
                return true;
            else if(block2->blockType() == Block::SegmentBlock)
                return false;

            /* Label Block is always displayed first */
            if(block1->blockType() == Block::LabelBlock)
                return true;
            else if(block2->blockType() == Block::LabelBlock)
                return false;

            /* Function < Instruction */
            if((block1->blockType() == Block::FunctionBlock) && (block2->blockType() == Block::InstructionBlock))
                return true;
            else if((block1->blockType() == Block::InstructionBlock) && (block2->blockType() == Block::FunctionBlock))
                return false;

            Q_ASSERT(false);
        }

        return block1->startAddress() < block2->startAddress();
    }
}
