#include "instructionitem.h"

namespace PrefSDK
{
    InstructionItem::InstructionItem(const Instruction::Ptr instr, const ProcessorDefinition::Ptr& procdef, ReferenceTable* reftable, QObject *parent): ListingItem(instr->address(), parent)
    {
        this->_instruction = instr;
        this->_processordef = procdef;
        this->_reftable = reftable;
    }

    Instruction::Ptr InstructionItem::instruction()
    {
        return this->_instruction;
    }

    ListingItem::ListingItemType InstructionItem::itemType()
    {
        return ListingItem::Instruction;
    }

    QString InstructionItem::stringValue()
    {
        return this->_processordef->output(this->_reftable, this->_instruction);
    }
}
