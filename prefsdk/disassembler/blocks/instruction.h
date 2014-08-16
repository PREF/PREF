#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include "prefsdk/datatype.h"
#include "prefsdk/disassembler/operand.h"
#include "prefsdk/disassembler/symbol.h"
#include "prefsdk/disassembler/blocks/block.h"

namespace PrefSDK
{
    namespace InstructionCategories
    {
        enum Category
        {
            Undefined,
            ControlFlow,
            StackManipulation,
            LoadStore,
            TestCompare,
            Arithmetic,
            Logical,
            IO,
            InterruptTrap,
            Privileged,
            NoOperation,
        };
    }

    namespace InstructionTypes
    {
        enum Type
        {
            Undefined              = 0x00000000,
            InterruptTrap          = 0x10000000,
            Privileged             = 0x20000000,
            Nop                    = 0x40000000,
            Stop                   = 0x80000000,

            Call                   = 0x00000001,
            Jump                   = 0x00000002,
            ConditionalCall        = 0x00000003,
            ConditionalJump        = 0x00000004,

            Push                   = 0x00000005,
            Pop                    = 0x00000006,

            Add                    = 0x00000007,
            Sub                    = 0x00000008,
            Mul                    = 0x00000009,
            Div                    = 0x0000001A,
            Mod                    = 0x0000001B,
            AddCarry               = 0x0000001C,
            SubCarry               = 0x0000001D,
            Asl                    = 0x0000001E,
            Asr                    = 0x0000001F,

            And                    = 0x00000020,
            Or                     = 0x00000021,
            Xor                    = 0x00000022,
            Not                    = 0x00080023,
            Lsl                    = 0x00000024,
            Lsr                    = 0x00000025,
            Rol                    = 0x00000026,
            Ror                    = 0x00000027,
            RolCarry               = 0x00000028,
            RorCarry               = 0x00000029,

            In                     = 0x0000002A,
            Out                    = 0x0000002B,
        };
    }

    class Instruction : public Block
    {
        Q_OBJECT

        public:
            typedef QList<Operand*> OperandList;

        public:
            explicit Instruction(uint64_t address, uint64_t offset, const SymbolTable& symboltable, QObject *parent = 0);
            bool contains(uint64_t address);
            void clearOperands();
            void cloneOperand(Operand* operand);
            Operand* addOperand(OperandTypes::Type operandtype, DataType::Type datatype);
            void removeOperand(int idx);
            void updateSize(uint64_t sz);
            InstructionCategories::Category category() const;
            InstructionTypes::Type type() const;
            uint64_t address() const;
            uint64_t offset() const;
            uint64_t size() const;
            uint64_t opCode() const;
            QString mnemonic() const;
            QString displayHexDump(QHexEditData* hexeditdata) const;
            QString displayOperands() const;
            int operandsCount() const;
            Operand* operand(int idx) const;
            void setOpCode(uint64_t opcode);
            void setMnemonic(const QString& mnemonic);
            void setCategory(InstructionCategories::Category category);
            void setType(InstructionTypes::Type type);
            void setFormat(const QString& s);

        public: /* Overriden Methods */
            virtual ListingTypes::Type objectType() const;
            virtual QString displayAddress() const;

        private:
            QString standardOperandFormat() const;
            QString customOperandformat() const;

        private:
            const SymbolTable& _symboltable;
            OperandList _operands;
            InstructionCategories::Category _category;
            InstructionTypes::Type _type;
            QString _opformat;
            QString _mnemonic;
            uint64_t _opcode;
            uint64_t _address;
            uint64_t _offset;
    };
}
#endif // INSTRUCTION_H
