#ifndef PREFSDK_INSTRUCTION_H
#define PREFSDK_INSTRUCTION_H

#include "prefsdk/type/datatype.h"
#include "prefsdk/type/datavalue.h"
#include "prefsdk/prefexception.h"
#include "prefsdk/libs/preflib/luahexeditdata.h"
#include "prefsdk/disassembler/operand.h"
#include "prefsdk/disassembler/blocks/block.h"
#include "prefsdk/disassembler/instruction/instructioncategory.h"
#include "prefsdk/disassembler/instruction/instructiontype.h"
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include <QtCore>

namespace PrefSDK
{
    class Instruction : public Block
    {
        Q_OBJECT

        Q_PROPERTY(lua_Integer address READ address)
        Q_PROPERTY(lua_Integer offset READ offset)
        Q_PROPERTY(lua_Integer size READ size)
        Q_PROPERTY(lua_Integer category READ category WRITE setCategory)
        Q_PROPERTY(lua_Integer type READ type WRITE setType)
        Q_PROPERTY(lua_Integer operandscount READ operandsCount)
        Q_PROPERTY(lua_Integer opcode READ opcode WRITE setOpcode)
        Q_PROPERTY(lua_Integer source READ source WRITE setSource)
        Q_PROPERTY(lua_Integer destination READ destination WRITE setDestination)
        Q_PROPERTY(lua_Integer base READ base WRITE setBase)
        Q_PROPERTY(lua_Integer displacement READ displacement WRITE setDisplacement)
        Q_PROPERTY(lua_Integer scale READ scale WRITE setScale)
        Q_PROPERTY(QString format READ format WRITE setFormat)
        Q_PROPERTY(QString mnemonic READ mnemonic WRITE setMnemonic)
        Q_PROPERTY(QString hexdump READ hexDump)
        Q_PROPERTY(PrefSDK::Operand* firstoperand READ firstOperand)
        Q_PROPERTY(PrefSDK::Operand* lastoperand READ lastOperand)
        Q_PROPERTY(PrefSDK::Operand sourceoperand READ sourceOperand)
        Q_PROPERTY(PrefSDK::Operand destinationoperand READ destinationOperand)
        Q_PROPERTY(PrefSDK::Operand baseoperand READ baseOperand)
        Q_PROPERTY(PrefSDK::Operand displacementoperand READ displacementOperand)
        Q_PROPERTY(PrefSDK::Operand scaleoperand READ scaleOperand)
        Q_PROPERTY(bool valid READ isValid)

        public:
            typedef QList<PrefSDK::Operand*> OperandList;

        public:
            explicit Instruction(const DataValue& address, const DataValue& offset, DataType::Type opcodetype, QHexEditData* hexeditdata, QObject *parent = 0);
            bool isJump() const;
            bool isCall() const;
            bool isValid() const;
            lua_Integer address() const;
            lua_Integer offset() const;
            lua_Integer size() const;
            lua_Integer category() const;
            lua_Integer type() const;
            lua_Integer operandsCount() const;
            lua_Integer opcode() const;
            lua_Integer source() const;
            lua_Integer destination() const;
            lua_Integer base() const;
            lua_Integer displacement() const;
            lua_Integer scale() const;
            const QString& format() const;
            const QString& mnemonic() const;
            QString hexDump() const;
            PrefSDK::Operand* firstOperand() const;
            PrefSDK::Operand* lastOperand() const;
            PrefSDK::Operand* sourceOperand() const;
            PrefSDK::Operand* destinationOperand() const;
            PrefSDK::Operand* baseOperand() const;
            PrefSDK::Operand* displacementOperand() const;
            PrefSDK::Operand* scaleOperand() const;
            void setCategory(lua_Integer category);
            void setType(lua_Integer type);
            void setOpcode(lua_Integer opc);
            void setSource(lua_Integer idx);
            void setDestination(lua_Integer idx);
            void setBase(lua_Integer idx);
            void setDisplacement(lua_Integer idx);
            void setScale(lua_Integer idx);
            void setFormat(const QString& s);
            void setMnemonic(const QString& mnemonic);            

        public:
            void setSize(const DataValue& size);
            const DataValue& opcodeValue() const;
            const DataValue& offsetValue() const;

        public slots:
            lua_Integer next(lua_Integer datatype);
            PrefSDK::Operand* addOperand(lua_Integer operandtype, lua_Integer datatype);
            PrefSDK::Operand* addOperand(lua_Integer operandtype, lua_Integer operanddescriptor, lua_Integer datatype);
            PrefSDK::Operand* operand(lua_Integer idx) const;
            void cloneOperand(QObject* op);
            void removeOperand(lua_Integer idx);
            void clearOperands();

        private:
            void checkDescriptor(Operand::Descriptor operanddescriptor);

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;

        public:
            static const QString INVALID_MNEMONIC;

        private:
            lua_Integer _source;
            lua_Integer _destination;
            lua_Integer _base;
            lua_Integer _displacement;
            lua_Integer _scale;

        private:
            QHexEditData* _hexeditdata;
            OperandList _operands;
            QString _format;
            QString _mnemonic;
            DataType::Type _opcodetype;
            DataValue _offset;
            DataValue _opcode;
            lua_Integer _category;
            lua_Integer _type;
    };
}
#endif // PREFSDK_INSTRUCTION_H
