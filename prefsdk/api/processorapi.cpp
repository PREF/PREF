#include "processorapi.h"

namespace PrefSDK
{
    namespace API
    {
        void DisassemblerDrawer_drawVirtualAddress(DisassemblerViewDrawer *__this, const char *segmentname, const char *address)
        {
            __this->drawVirtualAddress(QString::fromUtf8(segmentname), QString::fromUtf8(address));
        }

        void DisassemblerDrawer_drawHexDump(DisassemblerViewDrawer *__this, uint64_t offset, int dumplength, int maxwidth)
        {
            __this->drawHexDump(offset, dumplength, maxwidth);
        }

        void DisassemblerDrawer_drawMnemonic(DisassemblerViewDrawer *__this, int width, const char *mnemonic, int instructionfeatures)
        {
            __this->drawMnemonic(width, QString::fromUtf8(mnemonic), static_cast<InstructionFeatures::Features>(instructionfeatures));
        }

        void DisassemblerDrawer_drawString(DisassemblerViewDrawer *__this, const char *s)
        {
            __this->drawString(QString::fromUtf8(s));
        }

        void DisassemblerDrawer_drawImmediate(DisassemblerViewDrawer *__this, const char *s)
        {
            __this->drawImmediate(QString::fromUtf8(s));
        }

        void DisassemblerDrawer_drawAddress(DisassemblerViewDrawer *__this, const char *s)
        {
            __this->drawAddress(QString::fromUtf8(s));
        }
    }
}
