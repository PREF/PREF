#ifndef PREFSDK_INSTRUCTION_H
#define PREFSDK_INSTRUCTION_H

#include "prefsdk/type/datatype.h"
#include "prefsdk/disassembler/blocks/block.h"
#include "prefsdk/libs/qt/qtlua.h"

namespace PrefSDK
{
    class Instruction : public Block
    {
        Q_OBJECT

        public:
            explicit Instruction(const QtLua::LuaTable& tc, DataType::Type addresstype, QObject* parent = 0);
            void push() const;
            QString mnemonic() const;
            bool isJump() const;
            bool isCall() const;
            bool isDestinationValid() const;
            DataValue destination() const;

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;

        private:
            QtLua::LuaTable _instructiontable;

        private:
            DataType::Type _addresstype;
    };
}
#endif // PREFSDK_INSTRUCTION_H
