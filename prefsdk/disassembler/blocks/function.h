#ifndef PREFSDK_FUNCTION_H
#define PREFSDK_FUNCTION_H

#include <algorithm>
#include "block.h"
#include "prefsdk/disassembler/blocks/instruction.h"
#include "prefsdk/disassembler/references/reference.h"

namespace PrefSDK
{
    namespace FunctionTypes
    {
        enum Type
        {
            Function   = 0x00000000,
            EntryPoint = 0x10000000,
            Export     = 0x00001000,
            Import     = 0x00002000,
        };
    }

    class Function : public Block
    {
        Q_OBJECT

        public:
            typedef QList<Instruction*> InstructionList;
            typedef QSet<Reference*> ReferenceSet;

        public:
            explicit Function(FunctionTypes::Type type, uint64_t startaddress, QObject* parent = 0);
            void addInstruction(Instruction* instruction);
            void removeInstruction(Instruction* instruction);
            void replaceInstruction(Instruction* oldinstruction, Instruction* newinstruction);
            int indexOf(Instruction* instruction) const;
            Instruction* instruction(int idx);
            int instructionsCount() const;
            FunctionTypes::Type type() const;
            bool isEntryPoint() const;
            bool isImport() const;
            bool isExport() const;
            QString references() const;

        public: /* Overriden Methods */
            virtual ListingTypes::Type objectType() const;

        private:
            static bool sortInstructions(Instruction* instruction1, Instruction* instruction2);

        private:
            ReferenceSet _references;
            InstructionList _instructions;
            FunctionTypes::Type _type;
    };
}

#endif // PREFSDK_FUNCTION_H
