#include "disassemblerlisting.h"

namespace PrefSDK
{
    DisassemblerListing::DisassemblerListing(lua_State* l, ByteBuffer *bytebuffer, const ProcessorDefinition::Ptr& processordef, QObject* parent): QObject(parent), LuaCTable(l, "DisassembleListing")
    {
        this->_maxinstructionsize = 1;
        this->_bytebuffer = bytebuffer;
        this->_processordef = processordef;
        this->_referencetable = new ReferenceTable(l);

        connect(this->_referencetable, SIGNAL(codeReferenceAdded(lua_Integer)), this, SLOT(onCodeReferenceAdded(lua_Integer)));
    }

    void DisassemblerListing::addInstruction(const Instruction::Ptr& instruction)
    {
        if(instruction->instructionSize() > this->_maxinstructionsize)
            this->_maxinstructionsize = instruction->instructionSize();

        this->_listing.add(new InstructionItem(instruction, this->_processordef, this->_referencetable));
    }

    bool DisassemblerListing::isDisassembled(lua_Integer address)
    {
        return this->_listing.containsInstruction(address);
    }

    ListingItem* DisassemblerListing::item(int i)
    {
        return this->_listing[i];
    }

    ReferenceTable* DisassemblerListing::referenceTable()
    {
        return this->_referencetable;
    }

    ByteBuffer *DisassemblerListing::buffer()
    {
        return this->_bytebuffer;
    }

    int DisassemblerListing::itemCount()
    {
        return this->_listing.count();
    }

    int DisassemblerListing::maxInstructionSize()
    {
        return this->_maxinstructionsize;
    }

    void DisassemblerListing::printBacktrace()
    {
        for(int i = 0; i < this->itemCount(); i++)
        {
            ListingItem* li = this->item(i);

            if(li->itemType() == ListingItem::Instruction)
            {
                InstructionItem* ii = dynamic_cast<InstructionItem*>(li);
                QString instr = this->_processordef->output(this->_referencetable, ii->instruction());
                lua_Integer size = ii->instruction()->instructionSize();
                DebugDialog::instance()->out(ii->address(), 16, 4)->hexDump(this->_bytebuffer->read(ii->address(), size))->outWord(instr)->outWord("\tSize:")->out(size)->newLine()->exec();
            }
        }
    }

    DisassemblerListing::Ptr DisassemblerListing::create(lua_State *l, ByteBuffer* bytebuffer, const ProcessorDefinition::Ptr& processordef)
    {
        return DisassemblerListing::Ptr(new DisassemblerListing(l, bytebuffer, processordef));
    }

    void DisassemblerListing::onCodeReferenceAdded(lua_Integer address)
    {
        LabelItem* li = new LabelItem(QString::number(address, 16), address, this->_referencetable);
        this->_listing.add(li);
    }
}
