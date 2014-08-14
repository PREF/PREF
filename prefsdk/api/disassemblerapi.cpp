#include "disassemblerapi.h"

namespace PrefSDK
{
    namespace API
    {
        void Loader_register(const char *name, const char *author, const char *version, LoaderList::LoaderId loaderid)
        {
            LoaderList::registerLoader(QString::fromUtf8(name), QString::fromUtf8(author), QString::fromUtf8(version), loaderid);
        }

        void LoaderModel_setValid(LoaderListModel *__this, LoaderList::LoaderId loaderid)
        {
            __this->setValid(loaderid);
        }

        void Operand_setValueInt8(Operand *__this, int8_t value)
        {
            __this->setValueInt8(value);
        }

        void Operand_setValueInt16(Operand *__this, int16_t value)
        {
            __this->setValueInt16(value);
        }

        void Operand_setValueInt32(Operand *__this, int32_t value)
        {
            __this->setValueInt32(value);
        }

        void Operand_setValueInt64(Operand *__this, int64_t value)
        {
            __this->setValueInt64(value);
        }

        void Operand_setValueUInt8(Operand *__this, uint8_t value)
        {
            __this->setValueUInt8(value);
        }

        void Operand_setValueUInt16(Operand *__this, uint16_t value)
        {
            __this->setValueUInt16(value);
        }

        void Operand_setValueUInt32(Operand *__this, uint32_t value)
        {
            __this->setValueUInt32(value);
        }

        void Operand_setValueUInt64(Operand *__this, uint64_t value)
        {
            __this->setValueUInt64(value);
        }

        int8_t Operand_getValueInt8(Operand *__this)
        {
            return __this->valueInt8();
        }

        int16_t Operand_getValueInt16(Operand *__this)
        {
            return __this->valueInt16();
        }

        int32_t Operand_getValueInt32(Operand *__this)
        {
            return __this->valueInt32();
        }

        int64_t Operand_getValueInt64(Operand *__this)
        {
            return __this->valueInt64();
        }

        uint8_t Operand_getValueUInt8(Operand *__this)
        {
            return __this->valueUInt8();
        }

        uint16_t Operand_getValueUInt16(Operand *__this)
        {
            return __this->valueUInt16();
        }

        uint32_t Operand_getValueUInt32(Operand *__this)
        {
            return __this->valueUInt32();
        }

        uint64_t Operand_getValueUInt64(Operand *__this)
        {
            return __this->valueUInt64();
        }

        void Operand_setRegisterName(Operand *__this, const char *regname)
        {
            __this->setRegisterName(QString::fromUtf8(regname));
        }

        void Operand_setDisplayValue(Operand *__this, const char *value)
        {
            __this->setDisplayValue(QString::fromUtf8(value));
        }

        void Instruction_setOpCode(Instruction *__this, uint64_t opcode)
        {
            __this->setOpCode(opcode);
        }

        uint64_t Instruction_getOpCode(Instruction *__this)
        {
            return __this->opCode();
        }

        void Instruction_setMnemonic(Instruction *__this, const char *mnemonic)
        {
            __this->setMnemonic(QString::fromUtf8(mnemonic));
        }

        const char *Instruction_getMnemonic(Instruction *__this)
        {
            return __this->mnemonic().toUtf8().constData();
        }

        void Instruction_setCategory(Instruction *__this, int category)
        {
            __this->setCategory(static_cast<InstructionCategories::Category>(category));
        }

        int Instruction_getCategory(Instruction *__this)
        {
            return __this->category();
        }

        void Instruction_setType(Instruction *__this, int type)
        {
            __this->setType(static_cast<InstructionTypes::Type>(type));
        }

        int Instruction_getType(Instruction *__this)
        {
            return __this->type();
        }

        uint64_t Instruction_getSize(Instruction *__this)
        {
            return __this->size();
        }

        uint64_t Instruction_getAddress(Instruction *__this)
        {
            return __this->address();
        }

        uint64_t Instruction_getOffset(Instruction *__this)
        {
            return __this->offset();
        }

        void Instruction_updateSize(Instruction *__this, uint64_t size)
        {
            __this->updateSize(size);
        }

        Operand* Instruction_addOperand(Instruction *__this, int operandtype, int datatype)
        {
            return __this->addOperand(static_cast<OperandTypes::Type>(operandtype), static_cast<DataType::Type>(datatype));
        }

        void DisassemblerListing_addSegment(DisassemblerListing *__this, const char *name, int segmenttype, uint64_t startaddress, uint64_t endaddress, uint64_t baseoffset)
        {
            __this->addSegment(QString::fromUtf8(name), static_cast<SegmentTypes::Type>(segmenttype), startaddress, endaddress, baseoffset);
        }

        void Instruction_setFormat(Instruction *__this, const char *s)
        {
            __this->setFormat(QString::fromUtf8(s));
        }

        void DisassemblerListing_addReference(DisassemblerListing *__this, uint64_t srcaddress, uint64_t destaddress, int referencetype)
        {
            __this->addReference(srcaddress, destaddress, static_cast<ReferenceTypes::Type>(referencetype));
        }

        void DisassemblerListing_setSymbol(DisassemblerListing *__this, uint64_t address, int datatype, const char *name)
        {
            __this->setSymbol(address, static_cast<DataType::Type>(datatype), QString::fromUtf8(name));
        }

        bool DisassemblerListing_hasSymbol(DisassemblerListing *__this, uint64_t address)
        {
            return __this->symbolTable().contains(address);
        }

        const char* DisassemblerListing_getSymbolName(DisassemblerListing *__this, uint64_t address)
        {
            return __this->symbolTable()[address]->name().toUtf8().constData();
        }

        bool DisassemblerListing_hasMoreInstructions(DisassemblerListing *__this)
        {
            return __this->hasMoreInstructions();
        }

        uint64_t DisassemblerListing_pop(DisassemblerListing *__this)
        {
            return __this->pop();
        }

        void DisassemblerListing_push(DisassemblerListing *__this, uint64_t address, int referencetype)
        {
            __this->push(address, static_cast<ReferenceTypes::Type>(referencetype));
        }

        void DisassemblerListing_addEntryPoint(DisassemblerListing *__this, const char *name, uint64_t address)
        {
            __this->addEntryPoint(QString::fromUtf8(name), address);
        }

        Instruction *DisassemblerListing_addInstruction(DisassemblerListing *__this, uint64_t address)
        {
            return __this->addInstruction(address);
        }

        int DisassemblerListing_getSegmentCount(DisassemblerListing *__this)
        {
            return __this->segmentsCount();
        }

        const char* Operand_getRegisterName(Operand *__this)
        {
            return __this->registerName().toUtf8().constData();
        }

        const char* Operand_getDisplayValue(Operand *__this)
        {
            return __this->displayValue().toUtf8().constData();
        }

        void Instruction_removeOperand(Instruction *__this, int idx)
        {
            __this->removeOperand(idx - 1);
        }

        Operand *Instruction_getOperand(Instruction *__this, int idx)
        {
            return __this->operand(idx - 1);
        }

        int Instruction_operandsCount(Instruction *__this)
        {
            return __this->operandsCount();
        }

        void Instruction_clearOperands(Instruction *__this)
        {
            __this->clearOperands();
        }

        int Operand_getType(Operand *__this)
        {
            return __this->type();
        }

        int Operand_getDataType(Operand *__this)
        {
            return __this->dataType();
        }

        Function* DisassemblerListing_getFunction(DisassemblerListing *__this, int idx)
        {
            return __this->function(idx - 1);
        }

        int DisassemblerListing_getFunctionCount(DisassemblerListing *__this)
        {
            return __this->functionsCount();
        }

        int Function_getInstructionCount(Function *__this)
        {
            return __this->instructionsCount();
        }

        Instruction *Function_getInstruction(Function *__this, int idx)
        {
            return __this->instruction(idx - 1);
        }

        void Instruction_cloneOperand(Instruction *__this, Operand *operand)
        {
            __this->cloneOperand(operand);
        }

        Instruction *DisassemblerListing_mergeInstructions(DisassemblerListing* __this, Instruction *instruction1, Instruction *instruction2, const char *mnemonic, int instrcategory, int instrtype)
        {
            return __this->mergeInstructions(instruction1, instruction2, QString::fromUtf8(mnemonic), static_cast<InstructionCategories::Category>(instrcategory), static_cast<InstructionTypes::Type>(instrtype));
        }
    }
}
