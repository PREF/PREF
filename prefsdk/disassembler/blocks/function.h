#ifndef PREFSDK_FUNCTION_H
#define PREFSDK_FUNCTION_H

#include <algorithm>
#include "block.h"
#include "prefsdk/disassembler/blocks/instruction.h"
#include "prefsdk/disassembler/functiontype.h"

namespace PrefSDK
{
    class Function : public Block
    {
        Q_OBJECT

        public:
            explicit Function(FunctionType::Type type, const DataValue& startaddress, QObject* parent = 0);
            lua_Integer type() const;
            bool isEntryPoint() const;
            bool isImport() const;
            bool isExport() const;
            void setType(FunctionType::Type ft);

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;

        private:
            FunctionType::Type _type;
    };
}

#endif // PREFSDK_FUNCTION_H
