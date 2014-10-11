#include "disassemblerlisting.h"

namespace PrefSDK
{
    DisassemblerListing::DisassemblerListing(QHexEditData *hexeditdata, QObject *parent): QObject(parent), _blocksorted(false), _addresstype(DataType::Invalid), _hexeditdata(hexeditdata)
    {
        this->_referencetable = new ReferenceTable(this);
        this->_symboltable = new SymbolTable(this);
        this->_constanttable = new ConstantTable(this);
    }

    DataType::Type DisassemblerListing::addressType() const
    {
        return this->_addresstype;
    }

    void DisassemblerListing::setAddressType(DataType::Type addresstype)
    {
        this->_addresstype = addresstype;
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

    void DisassemblerListing::analyzeOperands()
    {
        for(InstructionMap::Iterator it = this->_instructions.begin(); it != this->_instructions.end(); it++)
        {
            Instruction* instruction = it.value();

            for(lua_Integer i = 0; i < instruction->operandsCount(); i++)
            {
                Operand* op = instruction->operand(i);

                if(op->type() == Operand::Address)
                {
                    qint64 idx = -1;

                    if(instruction->isJump() && !this->_symboltable->contains(op->operandValue()) && (idx = this->indexOf(op->operandValue(), Block::InstructionBlock)) != -1)
                    {
                        Reference::Type referencetype = ((instruction->type() == InstructionType::ConditionalJump) ? Reference::ConditionalJump : Reference::Jump);

                        if(!this->_referencetable->isReferenced(op->operandValue()))
                            this->createReference(op->operandValue(), instruction->startAddress(), referencetype, idx);

                        this->_symboltable->set(Symbol::Jump, op->operandValue(), QString("j_%1").arg(op->operandValue().toString(16)));
                    }
                    else
                        this->analyzeAddress(instruction, op->operandValue());
                }
            }
        }
    }

    bool DisassemblerListing::isDecoded(const DataValue& address) const
    {
        return this->_instructions.contains(address);
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

    void DisassemblerListing::createSegment(const QString &name, Segment::Type segmenttype, const DataValue& startaddress, const DataValue& size, const DataValue& baseoffset)
    {
        if(this->_segments.contains(startaddress))
            return;

        Segment* s = new Segment(name, segmenttype, startaddress, size, baseoffset, this);

        this->_segments[startaddress] = s;
        this->_blocks.append(s);
    }

    Function* DisassemblerListing::createFunction(FunctionType::Type functiontype, const DataValue &startaddress)
    {
        return this->createFunction(QString(), functiontype, startaddress);
    }

    Function* DisassemblerListing::createFunction(const QString &name, FunctionType::Type functiontype, const DataValue &startaddress)
    {
        Function* f = nullptr;

        if(this->_functions.contains(startaddress))
            return this->_functions[startaddress];

        if(name.isEmpty())
            f = new Function(functiontype, startaddress, this);
        else
            f = new Function(name, functiontype, startaddress, this);

        this->_functions[startaddress] = f;

        if(functiontype == FunctionType::EntryPointFunction)
        {
            this->_entrypoints.append(f);
            std::sort(this->_entrypoints.begin(), this->_entrypoints.end(), &DisassemblerListing::sortBlocks);
        }

        if(!this->_symboltable->contains(startaddress)) /* Apply Symbol, if needed */
            this->_symboltable->set(Symbol::Function, startaddress, f->name());

        this->_blocks.append(f);
        return f;
    }

    Instruction *DisassemblerListing::createInstruction(const DataValue& address, DataType::Type opcodetype)
    {
        if(this->_instructions.contains(address))
        {
            throw PrefException(QString("DisassemblerListing::createInstruction(): Instruction at %1h already exists").arg(address.toString(16)));
            return nullptr;
        }

        Segment* segment = this->findSegment(address);

        if(!segment)
            throw PrefException(QString("No segment for: %1").arg(address.toString(16)));

        Instruction* instruction = new Instruction(address, (address - segment->startAddress()) + segment->baseOffset(), opcodetype, this->_hexeditdata);
        this->_instructions[address] = instruction;
        this->_blocks.append(instruction);
        return instruction;
    }

    Segment *DisassemblerListing::findSegment(Block *block)
    {
        return this->findSegment(block->startAddress());
    }

    Segment *DisassemblerListing::findSegment(const DataValue &address)
    {
        if(this->_segments.contains(address))  /* We're lucky, return the block directly! */
            return this->_segments[address];

        for(SegmentMap::iterator it = this->_segments.begin(); it != this->_segments.end(); it++)
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

    Instruction *DisassemblerListing::findInstruction(const DataValue &address)
    {        
        if(this->_instructions.contains(address))  /* We're lucky, return the block directly! */
            return this->_instructions[address];

        for(InstructionMap::iterator it = this->_instructions.begin(); it != this->_instructions.end(); it++)
        {
            Instruction* i = it.value();

            if(i->contains(address))
                return i;
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

    QString DisassemblerListing::formatInstruction(Instruction *instruction)
    {
        if(!instruction->isValid())
            return this->formatInvalidInstruction(instruction);

        QString s = instruction->mnemonic() + " ";

        if(!instruction->format().isEmpty())
            s.append(this->formatInstructionCustom(instruction));
        else
        {
            for(int i = 0; i < instruction->operandsCount(); i++)
            {
                Operand* operand = instruction->operand(i);

                if(i > 0)
                    s.append(", ");

                s.append(this->formatOperand(instruction, operand));
            }
        }

        return s;
    }

    QString DisassemblerListing::formatInstructionCustom(Instruction *instruction)
    {
        int i = 0;
        QChar ch;
        QString s, format = instruction->format();

        while(i < format.length())
        {
            ch = format[i];

            switch(ch.toLatin1())
            {
                case '%':
                {
                    ch = format[++i];

                    if(ch.isNull())
                        break;

                    if(ch == '%')
                    {
                        s.append("%");
                        break;
                    }

                    bool ok = false;
                    int opidx = QString(ch).toInt(&ok);

                    if(!ok || (opidx < 0 || opidx > instruction->operandsCount()))
                    {
                        if(!ok)
                            throw PrefException(QString("Invalid Operand Format: Expected Integer not '%1'").arg(QString(ch)));
                        else
                            throw PrefException("Operand Index out of range");

                        continue;
                    }

                    Operand* operand = instruction->operand(opidx - 1);
                    s.append(this->formatOperand(instruction, operand));
                    break;
                }

                case '\0':
                    return s; /* Something wrong: reached EOS */

                default:
                {
                    s.append(ch);
                    break;
                }
            }

            i++;
        }

        return s;
    }

    QString DisassemblerListing::formatInvalidInstruction(Instruction *instruction)
    {
        return QString("db (%1)").arg(instruction->hexDump().replace(" ", "h, ").append("h"));
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

    Instruction *DisassemblerListing::replaceInstructions(QObject *b1, QObject *b2, const QString &mnemonic, lua_Integer category)
    {
        return this->replaceInstructions(b1, b2, mnemonic, category, InstructionType::Undefined);
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

    Instruction *DisassemblerListing::replaceInstructions(QObject* i1, QObject* i2, const QString& mnemonic, lua_Integer category, lua_Integer type)
    {
        Block* b1 = qobject_cast<Block*>(i1);
        Block* b2 = qobject_cast<Block*>(i2);

        if(b1->blockType() != Block::InstructionBlock)
        {
            throw PrefException("DisassemblerListing::replaceInstructions(): Argument 1 is not an instruction block");
            return nullptr;
        }

        if(b2->blockType() != Block::InstructionBlock)
        {
            throw PrefException("DisassemblerListing::replaceInstructions(): Argument 2 is not an instruction block");
            return nullptr;
        }

        if(!this->_instructions.contains(b1->startAddress()) || !this->_instructions.contains(b2->startAddress()))
        {
            if(!this->_instructions.contains(b1->startAddress()))
                throw PrefException("DisassemblerListing::replaceInstructions(): Argument 1 not in listing");

            if(!this->_instructions.contains(b2->startAddress()))
                throw PrefException("DisassemblerListing::replaceInstructions(): Argument 2 not in listing");

            return nullptr;
        }

        Instruction* instruction1 = qobject_cast<Instruction*>(b1);
        Instruction* newinstruction = new Instruction(instruction1->startAddress(), instruction1->offsetValue(), instruction1->opcodeValue().dataType(), this->_hexeditdata, this->_symboltable);
        newinstruction->setEndAddress(b2->endAddress());
        newinstruction->setMnemonic(mnemonic);
        newinstruction->setCategory(category);
        newinstruction->setType(type);

        qint64 idx = this->indexOf(instruction1);
        this->removeInstructions(instruction1, qobject_cast<Instruction*>(b2));
        this->_instructions[newinstruction->startAddress()] = newinstruction;
        this->_blocks.insert(idx, newinstruction);

        return newinstruction;
    }

    void DisassemblerListing::checkSort()
    {        
        if(!this->_blocksorted)
        {
            std::sort(this->_blocks.begin(), this->_blocks.end(), &DisassemblerListing::sortBlocks);
            this->_blocksorted = true;
        }
    }

    bool DisassemblerListing::pointsToString(const DataValue &address)
    {
        Segment* segment = this->findSegment(address);

        if(!segment || this->_segments.contains(address) || this->_functions.contains(address) || this->_instructions.contains(address)) /* Ignore code blocks */
            return false;

        DataValue offset = (address - segment->startAddress()) + segment->baseOffset();

        QHexEditDataReader reader(this->_hexeditdata);
        QString s = reader.readString(offset.compatibleValue<qint64>(), 4);

        return s.length() == 4;
    }

    void DisassemblerListing::analyzeAddress(Instruction* instruction, const DataValue &address)
    {
        Segment* segment = this->findSegment(address); /* Don't analyze weird address */

        if(!segment)
            return;

        this->_referencetable->addReference(address, instruction->startAddress(), Reference::Address);

        if(this->_symboltable->contains(address) && this->_symboltable->isType(address, Symbol::Address))
            return;

        if(this->pointsToString(address))
        {
            QHexEditDataReader reader(this->_hexeditdata);
            DataValue offset = (address - segment->startAddress()) + segment->baseOffset();
            QString s = reader.readString(offset.compatibleValue<qint64>());

            this->_stringsymbols.insert(address);
            this->_variables.insert(address);
            this->_symboltable->set(Symbol::String, address, DataValue::create(s.length(), this->_addresstype), DataType::AsciiString, QString("string_%1").arg(address.toString(16)));
        }
        else if(!this->_symboltable->contains(address))
        {
            this->_variables.insert(address);
            this->_symboltable->set(Symbol::Address, address, QString("data_%1").arg(address.toString(16)));
        }
    }

    QString DisassemblerListing::formatOperand(Instruction* instruction, Operand *operand)
    {
        Operand::Type optype = static_cast<Operand::Type>(operand->type());

        if((optype == Operand::Address) && this->_symboltable->contains(operand->operandValue()))
            return this->_symboltable->name(operand->operandValue());
        else if((optype == Operand::Immediate) && this->_constanttable->isConstant(instruction, operand->operandValue()))
            return this->_constanttable->name(instruction, operand->operandValue());
        else if(optype == Operand::Register)
            return "$" + operand->registerName();

        return operand->operandValue().toString(16) + "h";
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
            if(block1->blockType() == Block::ReferenceBlock)
                return true; /* Reference Block are always displayed first */

            if(block1->blockType() == Block::SegmentBlock)
                return true; /* Segment < Function | Instruction */

            if((block1->blockType() == Block::FunctionBlock) && (block2->blockType() == Block::InstructionBlock))
                return true; /* Function < Instruction */
        }

        return block1->startAddress() < block2->startAddress();
    }
}
