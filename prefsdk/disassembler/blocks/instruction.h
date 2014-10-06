#ifndef PREFSDK_INSTRUCTION_H
#define PREFSDK_INSTRUCTION_H

#include "prefsdk/type/datatype.h"
#include "prefsdk/type/datavalue.h"
#include "prefsdk/prefexception.h"
#include "prefsdk/libs/preflib/luahexeditdata.h"
#include "prefsdk/disassembler/operand.h"
#include "prefsdk/disassembler/symbol/symboltable.h"
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
        Q_PROPERTY(QString format READ format WRITE setFormat)
        Q_PROPERTY(QString mnemonic READ mnemonic WRITE setMnemonic)
        Q_PROPERTY(QString hexdump READ hexDump)
        Q_PROPERTY(PrefSDK::Operand* firstoperand READ firstOperand)
        Q_PROPERTY(PrefSDK::Operand* lastoperand READ lastOperand)

        public:
            typedef QList<PrefSDK::Operand*> OperandList;

        public:
            explicit Instruction(const DataValue& address, const DataValue& offset, DataType::Type opcodetype, QHexEditData* hexeditdata, const SymbolTable* symboltable, QObject *parent = 0);
            lua_Integer address() const;
            lua_Integer offset() const;
            lua_Integer size() const;
            lua_Integer category() const;
            lua_Integer type() const;
            lua_Integer operandsCount() const;
            lua_Integer opcode() const;
            const QString& format() const;
            const QString& mnemonic() const;
            QString hexDump() const;
            PrefSDK::Operand* firstOperand() const;
            PrefSDK::Operand* lastOperand() const;
            void setCategory(lua_Integer category);
            void setType(lua_Integer type);
            void setOpcode(lua_Integer opc);
            void setFormat(const QString& s);
            void setMnemonic(const QString& mnemonic);

        public:
            void setSize(const DataValue& size);

        public slots:
            lua_Integer next(lua_Integer datatype);
            PrefSDK::Operand* addOperand(lua_Integer operandtype, lua_Integer datatype);
            PrefSDK::Operand* operand(lua_Integer idx) const;
            void cloneOperand(QObject* op);
            void removeOperand(lua_Integer idx);
            void clearOperands();

        public:
            const DataValue& opcodeValue() const;
            const DataValue& offsetValue() const;

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;

        private:
            const SymbolTable* _symboltable;
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
