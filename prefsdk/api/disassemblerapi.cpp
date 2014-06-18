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

        Operand *Operand_create(int type)
        {
            return new Operand(static_cast<OperandTypes::Type>(type));
        }

        void Operand_setDisplayValue(Operand *__this, const char *value)
        {
            __this->setDisplayValue(value);
        }

        Instruction *Instruction_create(uint64_t va, uint64_t offset)
        {
            return new Instruction(va, offset);
        }

        void Instruction_addOperand(Instruction *__this, Operand *operand)
        {
            __this->addOperand(operand);
        }

        void Instruction_setCategory(Instruction *__this, int category)
        {
            __this->setCategory(static_cast<InstructionCategories::Category>(category));
        }

        void Instruction_setType(Instruction *__this, int type)
        {
            __this->setType(static_cast<InstructionTypes::Type>(type));
        }

        void Instruction_setMnemonic(Instruction *__this, const char *mnemonic)
        {
            __this->setMnemonic(QString::fromUtf8(mnemonic));
        }

        Function *Function_create(int functiontype, uint64_t startaddress, uint64_t endaddress)
        {
            return new Function(static_cast<FunctionTypes::Type>(functiontype), startaddress, endaddress);
        }

        void Function_addInstruction(Function *__this, Instruction *instruction)
        {
            __this->addInstruction(instruction);
        }

        void Instruction_setSize(Instruction *__this, uint64_t size)
        {
            __this->setSize(size);
        }

        Segment *Segment_create(const char *name, int segmenttype, uint64_t startaddress, uint64_t endaddress, uint64_t baseoffset)
        {
            return new Segment(QString::fromUtf8(name), static_cast<SegmentTypes::Type>(segmenttype), startaddress, endaddress, baseoffset);
        }

        void Segment_addFunction(Segment *__this, Function *f)
        {
            __this->addFunction(f);
        }

        void DisassemblerListing_addSegment(DisassemblerListing *__this, Segment *segment)
        {
            __this->addSegment(segment);
        }

        void Instruction_formatInstruction(Instruction *__this, const char *s)
        {
            __this->formatInstruction(QString::fromUtf8(s));
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
    }
}
