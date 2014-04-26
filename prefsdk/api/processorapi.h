#ifndef PREFSDK_API_PROCESSORAPI_H
#define PREFSDK_API_PROCESSORAPI_H

#include "qhexedit/qhexeditdata.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/disassembler/disassembler.h"
#include "views/formatview/disassemblerviewpage/disassemblerview/disassemblerviewdrawer.h"

namespace PrefSDK
{
    namespace API
    {
        /* InstructionFeature's Enum */
        extern "C" const int InstructionFeatures_Stop = InstructionFeatures::Stop;
        extern "C" const int InstructionFeatures_Call = InstructionFeatures::Call;
        extern "C" const int InstructionFeatures_Change1 = InstructionFeatures::Change1;
        extern "C" const int InstructionFeatures_Change2 = InstructionFeatures::Change2;
        extern "C" const int InstructionFeatures_Change3 = InstructionFeatures::Change3;
        extern "C" const int InstructionFeatures_Change4 = InstructionFeatures::Change4;
        extern "C" const int InstructionFeatures_Change5 = InstructionFeatures::Change5;
        extern "C" const int InstructionFeatures_Change6 = InstructionFeatures::Change6;
        extern "C" const int InstructionFeatures_Use1 = InstructionFeatures::Use1;
        extern "C" const int InstructionFeatures_Use2 = InstructionFeatures::Use2;
        extern "C" const int InstructionFeatures_Use3 = InstructionFeatures::Use3;
        extern "C" const int InstructionFeatures_Use4 = InstructionFeatures::Use4;
        extern "C" const int InstructionFeatures_Use5 = InstructionFeatures::Use5;
        extern "C" const int InstructionFeatures_Use6 = InstructionFeatures::Use6;
        extern "C" const int InstructionFeatures_Jump = InstructionFeatures::Jump;
        extern "C" const int InstructionFeatures_Shift = InstructionFeatures::Shift;

        /* OperandType's Enum */
        extern "C" const int OperandType_Void = OperandType::Void;
        extern "C" const int OperandType_Register = OperandType::Register;
        extern "C" const int OperandType_Memory = OperandType::Memory;
        extern "C" const int OperandType_Phrasse = OperandType::Phrase;
        extern "C" const int OperandType_Displacement = OperandType::Displacement;
        extern "C" const int OperandType_Immediate = OperandType::Immediate;
        extern "C" const int OperandType_JumpFar = OperandType::JumpFar;
        extern "C" const int OperandType_JumpNear = OperandType::JumpNear;
        extern "C" const int OperandType_CallFar = OperandType::CallFar;
        extern "C" const int OperandType_CallNear = OperandType::CallNear;

        extern "C"
        {
            /* DisassemblerView's API */
            void DisassemblerDrawer_drawVirtualAddress(DisassemblerViewDrawer* __this, const char* segmentname, const char* address);
            void DisassemblerDrawer_drawHexDump(DisassemblerViewDrawer* __this, uint64_t offset, int dumplength, int maxwidth);
            void DisassemblerDrawer_drawMnemonic(DisassemblerViewDrawer* __this, int width, const char* mnemonic, int instructionfeatures);
            void DisassemblerDrawer_drawImmediate(DisassemblerViewDrawer* __this, const char* s);
            void DisassemblerDrawer_drawAddress(DisassemblerViewDrawer* __this, const char* s);
            void DisassemblerDrawer_drawRegister(DisassemblerViewDrawer* __this, const char* s);
            void DisassemblerDrawer_drawString(DisassemblerViewDrawer* __this, const char* s);
        }
    }
}

#endif // PREFSDK_API_PROCESSORAPI_H
