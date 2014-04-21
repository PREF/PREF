#ifndef PREFSDK_API_PROCESSORAPI_H
#define PREFSDK_API_PROCESSORAPI_H

#include "prefsdk/disassembler/disassembler.h"
#include "prefsdk/format/formatlist.h"

namespace PrefSDK
{
    namespace API
    {
        /* DisassemblerSegment's Flags */
        extern "C" const uint64_t SegmentType_Data = DisassemblerSegment::Data;
        extern "C" const uint64_t SegmentType_Code = DisassemblerSegment::Code;

        /* Operand's Flags */
        extern "C" const uint64_t Operand_Void = Operand::Void;
        extern "C" const uint64_t Operand_Register = Operand::Register;
        extern "C" const uint64_t Operand_Memory = Operand::Memory;
        extern "C" const uint64_t Operand_Phrase = Operand::Phrase;
        extern "C" const uint64_t Operand_Immediate = Operand::Immediate;
        extern "C" const uint64_t Operand_JumpFar = Operand::JumpFar;
        extern "C" const uint64_t Operand_JumpNear = Operand::JumpNear;
        extern "C" const uint64_t Operand_CallFar = Operand::CallFar;
        extern "C" const uint64_t Operand_CallNear = Operand::CallNear;

        /* Instruction's Flags */
        extern "C" const uint64_t InstructionFeatures_Stop = Instruction::Stop;
        extern "C" const uint64_t InstructionFeatures_Call = Instruction::Call;
        extern "C" const uint64_t InstructionFeatures_Change1 = Instruction::Change1;
        extern "C" const uint64_t InstructionFeatures_Change2 = Instruction::Change2;
        extern "C" const uint64_t InstructionFeatures_Change3 = Instruction::Change3;
        extern "C" const uint64_t InstructionFeatures_Change4 = Instruction::Change4;
        extern "C" const uint64_t InstructionFeatures_Change5 = Instruction::Change5;
        extern "C" const uint64_t InstructionFeatures_Change6 = Instruction::Change6;
        extern "C" const uint64_t InstructionFeatures_Use1 = Instruction::Use1;
        extern "C" const uint64_t InstructionFeatures_Use2 = Instruction::Use2;
        extern "C" const uint64_t InstructionFeatures_Use3 = Instruction::Use3;
        extern "C" const uint64_t InstructionFeatures_Use4 = Instruction::Use4;
        extern "C" const uint64_t InstructionFeatures_Use5 = Instruction::Use5;
        extern "C" const uint64_t InstructionFeatures_Use6 = Instruction::Use6;
        extern "C" const uint64_t InstructionFeatures_Jump = Instruction::Jump;
        extern "C" const uint64_t InstructionFeatures_Shift = Instruction::Shift;

        /* CodeReferenceTable's Flags */
        extern "C" const uint64_t CodeReference_CallFar = ReferenceTable::CallFar;
        extern "C" const uint64_t CodeReference_CallNear = ReferenceTable::CallNear;
        extern "C" const uint64_t CodeReference_JumpFar = ReferenceTable::JumpFar;
        extern "C" const uint64_t CodeReference_JumpNear = ReferenceTable::JumpNear;

        /* DataReferenceTable's Flags */
        extern "C" const uint64_t DataReference_Offset = ReferenceTable::Offset;
        extern "C" const uint64_t DataReference_Read =  ReferenceTable::Read;
        extern "C" const uint64_t DataReference_Write = ReferenceTable::Write;

        extern "C"
        {
            void ProcessorLoader_addSegment(FormatList::FormatId formatid, const char* name, uint64_t segmenttype, uint64_t startoffset, uint64_t endoffset, uint64_t baseaddress);
            void ProcessorLoader_addEntry(FormatList::FormatId formatid, const char* name, uint64_t offset);
        }
    }
}

#endif // PREFSDK_API_PROCESSORAPI_H
