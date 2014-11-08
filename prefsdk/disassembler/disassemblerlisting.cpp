#include "disassemblerlisting.h"

namespace PrefSDK
{
    DisassemblerListing::DisassemblerListing(QHexEditData *hexeditdata, DataType::Type addresstype, QObject *parent): LogObject(parent), _blocksorted(false), _hexeditdata(hexeditdata), _formattree(nullptr), _addresstype(addresstype)
    {
        this->_symboltable = new SymbolTable(addresstype, this);
        this->_constanttable = new ConstantTable(this);
    }

    void DisassemblerListing::setLogger(Logger *logger)
    {
        LogObject::setLogger(logger);
        this->_symboltable->setLogger(logger);
    }

    void DisassemblerListing::setFormatTree(FormatTree *formattree)
    {
        this->_formattree = formattree;
        this->_formattree->setParent(this);
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

    void DisassemblerListing::createFunction(const DataValue &address, const DataValue &calleraddress, FunctionType::Type functiontype, const QString &name)
    {
        if(!this->isAddress(address))
        {
            this->warning(QString("Trying to creating an out of segment function at %1").arg(address.toString(16)));
            return;
        }

        if(this->_functions.contains(address))
        {
            this->_functions[address]->addSource(calleraddress);
            return;
        }

        Function* f = new Function(functiontype, address, this);

        if(!calleraddress.isNull())
            f->addSource(calleraddress);

        this->_functions[address] = f;

        if(functiontype == FunctionType::EntryPointFunction)
        {
            this->_entrypoints.append(f);
            std::sort(this->_entrypoints.begin(), this->_entrypoints.end(), &DisassemblerListing::sortBlocks);
        }

        this->_symboltable->set(Symbol::Function, address, calleraddress, name);
        this->_blocks.append(f);
    }

    void DisassemblerListing::createFunction(const DataValue &address, const DataValue &calleraddress, const QString &name)
    {
        this->createFunction(address, calleraddress, FunctionType::NormalFunction, name);
    }

    void DisassemblerListing::createFunction(const DataValue &address, const DataValue &calleraddress)
    {
        this->createFunction(address, calleraddress, QString("sub_%1").arg(address.toString(16)));
    }

    qint64 DisassemblerListing::length() const
    {
        return this->_blocks.length();
    }

    FormatTree *DisassemblerListing::formatTree()
    {
        return this->_formattree;
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

    void DisassemblerListing::createLabel(lua_Integer destaddress, lua_Integer calleraddress, const QString &name)
    {
        DataValue destaddressvalue = DataValue::create(destaddress, this->_addresstype);
        Segment* segment = this->findSegment(destaddressvalue);

        if(!segment)
            throw PrefException(QString("No segment for: %1").arg(destaddressvalue.toString(16)));

        Label* label = nullptr;
        DataValue calleraddressvalue = DataValue::create(calleraddress, this->_addresstype);

        if(!this->_labels.contains(destaddressvalue))
        {
            label = new Label(destaddressvalue, this);

            this->_labels[destaddressvalue] = label;
            this->_symboltable->set(Symbol::Label, destaddressvalue, calleraddressvalue, name);
            this->_blocks.append(label);
        }
        else
            label = this->_labels[destaddressvalue];

        label->addSource(calleraddressvalue);
    }

    void DisassemblerListing::createSegment(const QString &name, lua_Integer segmenttype, lua_Integer startaddress, lua_Integer size, lua_Integer baseoffset)
    {
        DataValue startaddressvalue = DataValue::create(startaddress, this->_addresstype);

        if(this->_segments.contains(startaddressvalue))
            return;

        DataValue sizevalue = DataValue::create(size, this->_addresstype);
        DataValue baseoffsetvalue = DataValue::create(baseoffset, this->_addresstype);

        Segment* s = new Segment(name, static_cast<Segment::Type>(segmenttype), startaddressvalue, sizevalue, baseoffsetvalue, this);
        this->_segments[startaddressvalue] = s;
        this->_blocks.append(s);
    }

    void DisassemblerListing::createFunction(lua_Integer address, lua_Integer calleraddress, lua_Integer functiontype, const QString &name)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        DataValue calleraddressvalue = DataValue::create(calleraddress, this->_addresstype);
        this->createFunction(addressvalue, calleraddressvalue, static_cast<FunctionType::Type>(functiontype), name);
    }

    void DisassemblerListing::createFunction(lua_Integer address, lua_Integer calleraddress, const QString &name)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        DataValue calleraddressvalue = DataValue::create(calleraddress, this->_addresstype);
        this->createFunction(addressvalue, calleraddressvalue, name);
    }

    void DisassemblerListing::createFunction(lua_Integer address, lua_Integer calleraddress)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        DataValue calleraddressvalue = DataValue::create(calleraddress, this->_addresstype);
        this->createFunction(addressvalue, calleraddressvalue);
    }

    void DisassemblerListing::createEntryPoint(lua_Integer address, const QString &name)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        this->createFunction(addressvalue, DataValue(), FunctionType::EntryPointFunction, name);
    }

    void DisassemblerListing::addInstruction(const PrefSDK::QtLua::LuaTable& instructiontable)
    {
        Instruction* instruction = new Instruction(instructiontable, this->_addresstype, this);
        DataValue address = instruction->startAddress();

        if(this->_instructions.contains(address))
        {
            instruction->deleteLater();
            return;
        }

        if(!this->isAddress(address))
        {
            instruction->deleteLater();
            throw PrefException(QString("Address %1 is invalid").arg(address.toString(16)));
            return;
        }

        this->_instructions[address] = instruction;
        this->_blocks.append(instruction);
    }

    void DisassemblerListing::setFunction(lua_Integer address, const QString &name)
    {
        this->setFunction(address, FunctionType::NormalFunction, name);
    }

    void DisassemblerListing::setFunction(lua_Integer address, lua_Integer functiontype, const QString &name)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        FunctionType::Type ft = static_cast<FunctionType::Type>(functiontype);

        if(!this->_functions.contains(addressvalue))
            return;

        this->_symboltable->set(Symbol::Function, addressvalue, DataValue(), name);
        Function* f = this->_functions[addressvalue];

        if(ft > f->type())
            f->setType(ft);
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

    bool DisassemblerListing::isAddress(lua_Integer address)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        return this->isAddress(addressvalue);
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

    const DisassemblerListing::FunctionMap &DisassemblerListing::functions() const
    {
        return this->_functions;
    }

    PrefSDK::SymbolTable *DisassemblerListing::symbolTable()
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
