#ifndef PREFSDK_INSTRUCTIONITEM_H
#define PREFSDK_INSTRUCTIONITEM_H

#include "listingitem.h"
#include "prefsdk/disassembler/referencetable.h"
#include "prefsdk/disassembler/processor/instruction.h"
#include "prefsdk/disassembler/processor/processordefinition.h"

namespace PrefSDK
{
    class InstructionItem : public ListingItem
    {
        Q_OBJECT

        public:
            explicit InstructionItem(const Instruction::Ptr instr, const ProcessorDefinition::Ptr& procdef, ReferenceTable* reftable, QObject *parent = 0);
            Instruction::Ptr instruction();
            virtual ListingItemType itemType();
            virtual QString stringValue();

        private:
            Instruction::Ptr _instruction;
            ReferenceTable* _reftable;
            ProcessorDefinition::Ptr _processordef;
    };
}

#endif // PREFSDK_INSTRUCTIONITEM_H
