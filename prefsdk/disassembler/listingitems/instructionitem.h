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
            explicit InstructionItem(Instruction* instr, const ProcessorDefinition::Ptr& procdef, ReferenceTable* reftable, QObject *parent = 0);
            Instruction* instruction();
            virtual ListingItems::Type itemType();
            virtual QString stringValue();

        private:
            Instruction* _instruction;
            ReferenceTable* _reftable;
            ProcessorDefinition::Ptr _processordef;
    };
}

#endif // PREFSDK_INSTRUCTIONITEM_H
