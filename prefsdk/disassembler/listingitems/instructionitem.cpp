#include "instructionitem.h"

namespace PrefSDK
{
    InstructionItem::InstructionItem(Instruction *instr, const ProcessorDefinition::Ptr& procdef, ReferenceTable* reftable, QObject *parent): ListingItem(instr->address(), parent)
    {
        this->_instruction = instr;
        this->_processordef = procdef;
        this->_reftable = reftable;
    }

    Instruction* InstructionItem::instruction()
    {
        return this->_instruction;
    }

    ListingItems::Type InstructionItem::itemType()
    {
        return ListingItems::Instruction;
    }

    QString InstructionItem::stringValue()
    {
        //NOTE: return this->_processordef->output(this->_reftable, this->_instruction);
    }
}
