#ifndef PREFSDK_API_DISASSEMBLERAPI_H
#define PREFSDK_API_DISASSEMBLERAPI_H

#include "qhexedit/qhexeditdata.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/disassembler/loader/loaderlist.h"
#include "prefsdk/disassembler/operand.h"
#include "prefsdk/disassembler/instruction.h"
#include "prefsdk/disassembler/blocks/function.h"
#include "prefsdk/disassembler/crossreferences/referencetable.h"
#include "viewmodels/loaderlistmodel/loaderlistmodel.h"
#include "views/disassemblerview/disassemblerlisting.h"

namespace PrefSDK
{
    namespace API
    {
        /* InstructionCategories' Enum */
        extern "C" const int InstructionCategory_Undefined = InstructionCategories::Undefined;
        extern "C" const int InstructionCategory_ControlFlow = InstructionCategories::ControlFlow;
        extern "C" const int InstructionCategory_StackManipulation = InstructionCategories::StackManipulation;
        extern "C" const int InstructionCategory_LoadStore = InstructionCategories::LoadStore;
        extern "C" const int InstructionCategory_TestCompare = InstructionCategories::TestCompare;
        extern "C" const int InstructionCategory_Arithmetic = InstructionCategories::Arithmetic;
        extern "C" const int InstructionCategory_Logical = InstructionCategories::Logical;
        extern "C" const int InstructionCategory_IO = InstructionCategories::IO;
        extern "C" const int InstructionCategory_InterruptTrap = InstructionCategories::InterruptTrap;
        extern "C" const int InstructionCategory_Privileged = InstructionCategories::Privileged;
        extern "C" const int InstructionCategory_NoOperation = InstructionCategories::NoOperation;

        /* InstructionTypes' Enum */
        extern "C" const int InstructionType_Undefined = InstructionTypes::Undefined;
        extern "C" const int InstructionType_InterruptTrap = InstructionTypes::InterruptTrap;
        extern "C" const int InstructionType_Privileged = InstructionTypes::Privileged;
        extern "C" const int InstructionType_Nop = InstructionTypes::Nop;
        extern "C" const int InstructionType_Stop = InstructionTypes::Stop;
        extern "C" const int InstructionType_Call = InstructionTypes::Call;
        extern "C" const int InstructionType_Jump = InstructionTypes::Jump;
        extern "C" const int InstructionType_ConditionalCall = InstructionTypes::ConditionalCall;
        extern "C" const int InstructionType_ConditionalJump = InstructionTypes::ConditionalJump;
        extern "C" const int InstructionType_Push = InstructionTypes::Push;
        extern "C" const int InstructionType_Pop = InstructionTypes::Pop;
        extern "C" const int InstructionType_Add = InstructionTypes::Add;
        extern "C" const int InstructionType_Sub = InstructionTypes::Sub;
        extern "C" const int InstructionType_Mul = InstructionTypes::Mul;
        extern "C" const int InstructionType_Div = InstructionTypes::Div;
        extern "C" const int InstructionType_Mod = InstructionTypes::Mod;
        extern "C" const int InstructionType_AddCarry = InstructionTypes::AddCarry;
        extern "C" const int InstructionType_SubCarry = InstructionTypes::SubCarry;
        extern "C" const int InstructionType_Asl = InstructionTypes::Asl;
        extern "C" const int InstructionType_Asr = InstructionTypes::Asr;
        extern "C" const int InstructionType_And = InstructionTypes::And;
        extern "C" const int InstructionType_Or = InstructionTypes::Or;
        extern "C" const int InstructionType_Xor = InstructionTypes::Xor;
        extern "C" const int InstructionType_Not = InstructionTypes::Not;
        extern "C" const int InstructionType_Lsl = InstructionTypes::Lsl;
        extern "C" const int InstructionType_Lsr = InstructionTypes::Lsr;
        extern "C" const int InstructionType_Rol = InstructionTypes::Rol;
        extern "C" const int InstructionType_Ror = InstructionTypes::Ror;
        extern "C" const int InstructionType_RolCarry = InstructionTypes::RolCarry;
        extern "C" const int InstructionType_RorCarry = InstructionTypes::RorCarry;
        extern "C" const int InstructionType_In = InstructionTypes::In;
        extern "C" const int InstructionType_Out = InstructionTypes::Out;

        /* OperandTypes' Enum */
        extern "C" const int OperandType_Undefined = OperandTypes::Undefined;
        extern "C" const int OperandType_Register = OperandTypes::Register;
        extern "C" const int OperandType_Immediate = OperandTypes::Immediate;
        extern "C" const int OperandType_Address = OperandTypes::Address;
        extern "C" const int OperandType_Expression = OperandTypes::Expression;

        /* SegmentTypes' Enum */
        extern "C" const int SegmentType_Code = SegmentTypes::Code;
        extern "C" const int SegmentType_Data = SegmentTypes::Data;

        /* FunctionTypes' Enum */
        extern "C" const int FunctionType_Function = FunctionTypes::Function;
        extern "C" const int FunctionType_EntryPoint = FunctionTypes::EntryPoint;
        extern "C" const int FunctionType_Export = FunctionTypes::Export;
        extern "C" const int FunctionType_Import = FunctionTypes::Import;

        /* ReferenceTypes' Enum */
        extern "C" const int ReferenceType_Flow = ReferenceTypes::Flow;
        extern "C" const int ReferenceType_Code = ReferenceTypes::Code;
        extern "C" const int ReferenceType_Data = ReferenceTypes::Data;
        extern "C" const int ReferenceType_Unconditional = ReferenceTypes::Unconditional;
        extern "C" const int ReferenceType_Conditional = ReferenceTypes::Conditional;
        extern "C" const int ReferenceType_Call = ReferenceTypes::Call;
        extern "C" const int ReferenceType_ConditionalCall = ReferenceTypes::ConditionalCall;
        extern "C" const int ReferenceType_Jump = ReferenceTypes::Jump;
        extern "C" const int ReferenceType_ConditionalJump = ReferenceTypes::ConditionalJump;
        extern "C" const int ReferenceType_Address = ReferenceTypes::Address;
        extern "C" const int ReferenceType_Read = ReferenceTypes::Read;
        extern "C" const int ReferenceType_Write = ReferenceTypes::Write;

        extern "C"
        {
            /* Loader's API */
            void Loader_register(const char* name, const char* author, const char* version, LoaderList::LoaderId loaderid);

            /* LoaderModel's API */
            void LoaderModel_setValid(LoaderListModel* __this, LoaderList::LoaderId loaderid);

            /* Operand's API */
            Operand* Operand_create(int type);
            void Operand_setDisplayValue(Operand* __this, const char* value);

            /* Instruction's API */
            Instruction* Instruction_create(uint64_t va, uint64_t offset);
            void Instruction_addOperand(Instruction* __this, Operand* operand);
            void Instruction_setSize(Instruction* __this, uint64_t size);
            void Instruction_setMnemonic(Instruction *__this, const char* mnemonic);
            void Instruction_setCategory(Instruction* __this, int category);
            void Instruction_setType(Instruction* __this, int type);
            void Instruction_formatInstruction(Instruction *__this, const char* s);

            /* Function's API */
            Function* Function_create(int functiontype, const char* name, uint64_t startaddress, uint64_t endaddress);
            void Function_addReference(Function* __this, uint64_t address);
            void Function_addInstruction(Function* __this, Instruction* instruction);

            /* Segment's API */
            Segment *Segment_create(const char *name, int segmenttype, uint64_t startaddress, uint64_t endaddress, uint64_t baseoffset);
            void Segment_addFunction(Segment* __this, Function* f);

            /* DisassemblerListing's API */
            void DisassemblerListing_addSegment(DisassemblerListing* __this, Segment* segment);

            /* ReferenceTable's API */
            void ReferenceTable_addReference(ReferenceTable* __this, uint64_t address, uint64_t referencedby);
        }
    }
}

#endif // PREFSDK_API_DISASSEMBLERAPI_H
