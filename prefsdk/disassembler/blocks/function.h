#ifndef PREFSDK_FUNCTION_H
#define PREFSDK_FUNCTION_H

#include <algorithm>
#include "block.h"
#include "prefsdk/disassembler/instruction.h"

namespace PrefSDK
{
    namespace ReferenceTypes
    {
        enum Type
        {
            Flow            = 0x00000000,
            Code            = 0x10000000,
            Data            = 0x20000000,

            Unconditional   = 0x01000000,
            Conditional     = 0x02000000,

            Call            = Code | Unconditional | 0x00000001,
            ConditionalCall = Code | Conditional   | 0x00000002,

            Jump            = Code | Unconditional | 0x00000004,
            ConditionalJump = Code | Conditional   | 0x00000008,

            Address         = Data | 0x00000001,
            Read            = Data | 0x00000002,
            Write           = Data | 0x00000004,
        };
    }

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
            void addReference(uint64_t address, ReferenceTypes::Type referencetype);
            void addInstruction(Instruction* instruction);
            int indexOf(Instruction* instruction) const;
            Instruction* instruction(int idx);
            int instructionsCount() const;
            FunctionTypes::Type type() const;
            const QString& name() const;
            QString references() const;

        public: /* Overriden Methods */
            virtual ListingTypes::Type objectType() const;

        private:
            ReferenceMap _references;
            AddressList _referencelist;
            AddressList _addresslist;
            InstructionMap _instructions;
            FunctionTypes::Type _type;
            QString _name;
    };
}

#endif // PREFSDK_FUNCTION_H
