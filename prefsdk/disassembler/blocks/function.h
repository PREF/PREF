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
            explicit Function(const QString& name, FunctionTypes::Type type, const DataValue& startaddress, QObject* parent = 0);
            explicit Function(FunctionTypes::Type type, const DataValue& startaddress, QObject* parent = 0);
            const QString& name() const;
            FunctionTypes::Type type() const;
            bool isEntryPoint() const;
            bool isImport() const;
            bool isExport() const;

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;

        private:
            ReferenceSet _references;
            InstructionList _instructions;
            QString _name;
            FunctionTypes::Type _type;
    };
}

#endif // PREFSDK_FUNCTION_H
