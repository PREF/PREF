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
        extern "C"
        {
            /* DisassemblerView's API */
            void DisassemblerDrawer_drawAddress(DisassemblerViewDrawer* __this, const char* segmentname, const char* address);
            void DisassemblerDrawer_drawHexDump(DisassemblerViewDrawer* __this, uint64_t offset, int dumplength, int maxwidth);
            void DisassemblerDrawer_drawMnemonic(DisassemblerViewDrawer* __this, int width, const char* mnemonic);
            void DisassemblerDrawer_drawString(DisassemblerViewDrawer* __this, const char* s);
        }
    }
}

#endif // PREFSDK_API_PROCESSORAPI_H
