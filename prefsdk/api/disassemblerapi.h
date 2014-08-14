#ifndef PREFSDK_API_DISASSEMBLERAPI_H
#define PREFSDK_API_DISASSEMBLERAPI_H

#include "qhexedit/qhexeditdata.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/disassembler/loader/loaderlist.h"
#include "prefsdk/disassembler/operand.h"
#include "prefsdk/disassembler/instruction.h"
#include "prefsdk/disassembler/blocks/function.h"
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
            int Operand_getType(Operand* __this);
            int Operand_getDataType(Operand* __this);
            int8_t Operand_getValueInt8(Operand* __this);
            int16_t Operand_getValueInt16(Operand* __this);
            int32_t Operand_getValueInt32(Operand* __this);
            int64_t Operand_getValueInt64(Operand* __this);
            uint8_t Operand_getValueUInt8(Operand* __this);
            uint16_t Operand_getValueUInt16(Operand* __this);
            uint32_t Operand_getValueUInt32(Operand* __this);
            uint64_t Operand_getValueUInt64(Operand* __this);
            const char* Operand_getRegisterName(Operand* __this);
            const char* Operand_getDisplayValue(Operand* __this);
            void Operand_setValueInt8(Operand* __this, int8_t value);
            void Operand_setValueInt16(Operand* __this, int16_t value);
            void Operand_setValueInt32(Operand* __this, int32_t value);
            void Operand_setValueInt64(Operand* __this, int64_t value);
            void Operand_setValueUInt8(Operand* __this, uint8_t value);
            void Operand_setValueUInt16(Operand* __this, uint16_t value);
            void Operand_setValueUInt32(Operand* __this, uint32_t value);
            void Operand_setValueUInt64(Operand* __this, uint64_t value);
            void Operand_setRegisterName(Operand* __this, const char* regname);
            void Operand_setDisplayValue(Operand* __this, const char* value);

            /* Instruction's API */
            void Instruction_setOpCode(Instruction* __this, uint64_t opcode);
            uint64_t Instruction_getOpCode(Instruction* __this);
            void Instruction_setMnemonic(Instruction* __this, const char* mnemonic);
            const char* Instruction_getMnemonic(Instruction* __this);
            void Instruction_setCategory(Instruction* __this, int category);
            int Instruction_getCategory(Instruction* __this);
            void Instruction_setType(Instruction* __this, int type);
            int Instruction_getType(Instruction* __this);
            uint64_t Instruction_getSize(Instruction* __this);
            uint64_t Instruction_getAddress(Instruction* __this);
            uint64_t Instruction_getOffset(Instruction* __this);
            void Instruction_updateSize(Instruction* __this, uint64_t size);
            void Instruction_setMnemonic(Instruction *__this, const char* mnemonic);
            void Instruction_setCategory(Instruction* __this, int category);
            void Instruction_setType(Instruction* __this, int type);
            void Instruction_setFormat(Instruction *__this, const char* s);
            Operand* Instruction_addOperand(Instruction* __this, int operandtype, int datatype);
            void Instruction_removeOperand(Instruction* __this, int idx);
            Operand* Instruction_getOperand(Instruction* __this, int idx);
            int Instruction_operandsCount(Instruction* __this);
            void Instruction_clearOperands(Instruction* __this);
            void Instruction_cloneOperand(Instruction* __this, Operand* operand);

            /* Function's API */
            int Function_getInstructionCount(Function* __this);
            Instruction* Function_getInstruction(Function* __this, int idx);

            /* DisassemblerListing's API */
            void DisassemblerListing_addSegment(DisassemblerListing* __this, const char *name, int segmenttype, uint64_t startaddress, uint64_t endaddress, uint64_t baseoffset);
            void DisassemblerListing_addEntryPoint(DisassemblerListing* __this, const char* name, uint64_t address);
            Function *DisassemblerListing_getFunction(DisassemblerListing* __this, int idx);
            Instruction* DisassemblerListing_addInstruction(DisassemblerListing* __this, uint64_t address);
            void DisassemblerListing_addReference(DisassemblerListing* __this, uint64_t srcaddress, uint64_t destaddress, int referencetype);
            void DisassemblerListing_setSymbol(DisassemblerListing* __this, uint64_t address, int datatype, const char* name);
            bool DisassemblerListing_hasSymbol(DisassemblerListing* __this, uint64_t address);
            bool DisassemblerListing_hasMoreInstructions(DisassemblerListing* __this);
            int DisassemblerListing_getSegmentCount(DisassemblerListing* __this);
            int DisassemblerListing_getFunctionCount(DisassemblerListing* __this);
            uint64_t DisassemblerListing_pop(DisassemblerListing* __this);
            void DisassemblerListing_push(DisassemblerListing* __this, uint64_t address, int referencetype);
            const char* DisassemblerListing_getSymbolName(DisassemblerListing* __this, uint64_t address);
            Instruction* DisassemblerListing_mergeInstructions(DisassemblerListing* __this, Instruction* instruction1, Instruction* instruction2, const char* mnemonic, int instrcategory, int instrtype);
        }
    }
}

#endif // PREFSDK_API_DISASSEMBLERAPI_H
