#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QtCore>
#include "operand.h"
#include "listingobject.h"
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"

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

    class Instruction : public ListingObject
    {
        Q_OBJECT

        public:
            typedef QList<Operand*> OperandList;

        public:
            explicit Instruction(uint64_t address, uint64_t offset, QObject *parent = 0);
            void addOperand(Operand* operand);
            void setSize(uint64_t sz);
            void formatInstruction(const QString& s);
            InstructionCategories::Category category() const;
            InstructionTypes::Type type() const;
            uint64_t address() const;
            uint64_t offset() const;
            uint64_t size() const;
            QString mnemonic() const;
            QString displayHexDump(QHexEditData* hexeditdata) const;
            QString displayOperands() const;
            int operandsCount() const;
            Operand* operand(int idx) const;
            void setMnemonic(const QString& mnemonic);
            void setCategory(InstructionCategories::Category category);
            void setType(InstructionTypes::Type type);

        public: /* Overriden Methods */
            virtual ListingTypes::Type objectType() const;
            virtual QString displayAddress() const;

        private:
            OperandList _operands;
            InstructionCategories::Category _category;
            InstructionTypes::Type _type;
            QString _displayoperand;
            QString _mnemonic;
            uint64_t _address;
            uint64_t _offset;
            uint64_t _size;
    };
}
#endif // INSTRUCTION_H
