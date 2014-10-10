#ifndef PREFSDK_FUNCTION_H
#define PREFSDK_FUNCTION_H

#include <algorithm>
#include "block.h"
#include "prefsdk/disassembler/blocks/instruction.h"
#include "prefsdk/disassembler/references/reference.h"
#include "prefsdk/disassembler/functiontype.h"

namespace PrefSDK
{
    class Function : public Block
    {
        Q_OBJECT

        Q_PROPERTY(lua_Integer type READ type WRITE setType)

        public:
            typedef QList<Instruction*> InstructionList;
            typedef QSet<Reference*> ReferenceSet;

        public:
            explicit Function(const QString& name, FunctionType::Type type, const DataValue& startaddress, QObject* parent = 0);
            explicit Function(FunctionType::Type type, const DataValue& startaddress, QObject* parent = 0);
            const QString& name() const;
            lua_Integer type() const;
            bool isEntryPoint() const;
            bool isImport() const;
            bool isExport() const;
            void setType(lua_Integer ft);

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;

        private:
            ReferenceSet _references;
            InstructionList _instructions;
            QString _name;
            FunctionType::Type _type;
    };
}

#endif // PREFSDK_FUNCTION_H
