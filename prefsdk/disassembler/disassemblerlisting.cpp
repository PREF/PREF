#include "disassemblerlisting.h"

namespace PrefSDK
{
    DisassemblerListing::DisassemblerListing(QHexEditData* hexeditdata, const ProcessorDefinition::Ptr& processordef, QObject* parent): QObject(parent)
    {
        this->_maxinstructionsize = 1;
        this->_hexeditdata = hexeditdata;
        this->_processordef = processordef;
        this->_referencetable = new ReferenceTable();

        connect(this->_referencetable, SIGNAL(codeReferenceAdded(uint64_t)), this, SLOT(onCodeReferenceAdded(uint64_t)));
    }

    void DisassemblerListing::addInstruction(Instruction* instruction)
    {
        if(instruction->instructionSize() > this->_maxinstructionsize)
            this->_maxinstructionsize = instruction->instructionSize();

        this->_listing.add(new InstructionItem(instruction, this->_processordef, this->_referencetable));
    }

    bool DisassemblerListing::isDisassembled(uint64_t address)
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

    QHexEditData *DisassemblerListing::buffer()
    {
        return this->_hexeditdata;
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

            if(li->itemType() == ListingItems::Instruction)
            {
                InstructionItem* ii = dynamic_cast<InstructionItem*>(li);
                QString instr = this->_processordef->output(this->_referencetable, ii->instruction());
                lua_Integer size = ii->instruction()->instructionSize();
                DebugDialog::instance()->out(ii->address(), 16, 4)->hexDump(this->_hexeditdata->read(ii->address(), size))->outWord(instr)->outWord("\tSize:")->out(size)->newLine()->exec();
            }
        }
    }

    void DisassemblerListing::onCodeReferenceAdded(uint64_t address)
    {
        LabelItem* li = new LabelItem(QString::number(address, 16), address, this->_referencetable);
        this->_listing.add(li);
    }
}
