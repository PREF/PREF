#include "disassemblerhelper.h"

DisassemblerHelper::DisassemblerHelper(QObject *parent): QObject(parent)
{
    this->_cancontinue = true;
}

void DisassemblerHelper::stop()
{
    this->_cancontinue = false;
}

DisassemblerListing::Ptr DisassemblerHelper::listing()
{
    return this->_listing;
}

void DisassemblerHelper::run(ByteBuffer *bytebuffer, FormatDefinition::Ptr formatdefinition, DisassemblerLoader::Ptr loader)
{
    this->_cancontinue = true;

    try
    {
        ProcessorDefinition::Ptr processor = loader->processor();
        this->_listing = DisassemblerListing::create(formatdefinition->state(), bytebuffer, processor);

        for(int i = 0; i < loader->entryCount(); i++)
        {
            AddressQueue::Ptr addrqueue = AddressQueue::create(formatdefinition->state());
            addrqueue->pushBack(loader->entryAddress(i));

            while(this->_cancontinue && !addrqueue->isEmpty())
            {
                lua_Integer address = addrqueue->popBack();

                if(this->_listing->isDisassembled(address))
                    continue;

                if(!loader->inSegment(address))
                {
                    DebugDialog::instance()->out("Trying to disassemble no code segment at address: ")->out(address, 16)->newLine(2);
                    this->_listing->printBacktrace();
                    break;
                }

                Instruction::Ptr instr = Instruction::create(formatdefinition->state(), address, address, bytebuffer);
                lua_Integer sz = processor->analyze(instr);

                if(sz)
                    processor->emulate(addrqueue, this->_listing->referenceTable(), instr);
                else
                    addrqueue->pushFront(address + 1); /* Got an Invalid Instruction: Try To Disassemble Next Byte */

                this->_listing->addInstruction(instr);
            }
        }
    }
    catch(LuaException&)
    {
        this->_listing->printBacktrace();
    }

    emit finished();
}
