#ifndef PREFSDK_FUNCTION_H
#define PREFSDK_FUNCTION_H

#include <algorithm>
#include "block.h"
#include "prefsdk/disassembler/instruction.h"
#include "prefsdk/disassembler/crossreferences/referencetable.h"

namespace PrefSDK
{
    namespace FunctionTypes
    {
        enum Type
        {
            Function,
            EntryPoint,
            Export,
            Import,
        };
    }

    class Function : public Block
    {
        Q_OBJECT

        public:
            typedef QList<uint64_t> AddressList;
            typedef QHash<uint64_t, Instruction*> InstructionMap;
            typedef QHash<uint64_t, ReferenceTypes::Type> ReferenceMap;

        public:
            explicit Function(FunctionTypes::Type type, const QString& name, uint64_t startaddress, uint64_t endaddress, QObject* parent = 0);
            void addReference(uint64_t address);
            void addInstruction(Instruction* instruction);
            int indexOf(Instruction* instruction) const;
            Instruction* instruction(int idx);
            int instructionsCount() const;
            FunctionTypes::Type type() const;
            const QString& name() const;

        public: /* Overriden Methods */
            virtual ListingTypes::Type objectType() const;

        private:
            ReferenceMap _references;
            AddressList _addresslist;
            InstructionMap _instructions;
            FunctionTypes::Type _type;
            QString _name;
    };
}

#endif // PREFSDK_FUNCTION_H
