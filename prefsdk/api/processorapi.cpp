#include "processorapi.h"

namespace PrefSDK
{
    namespace API
    {
        void DisassemblerDrawer_drawAddress(DisassemblerViewDrawer *__this, const char *segmentname, const char *address)
        {
            __this->drawAddress(QString::fromUtf8(segmentname), QString::fromUtf8(address));
        }

        void DisassemblerDrawer_drawHexDump(DisassemblerViewDrawer *__this, uint64_t offset, int dumplength, int maxwidth)
        {
            __this->drawHexDump(offset, dumplength, maxwidth);
        }

        void DisassemblerDrawer_drawMnemonic(DisassemblerViewDrawer *__this, int width, const char *mnemonic)
        {
            __this->drawMnemonic(width, QString::fromUtf8(mnemonic));
        }

        void DisassemblerDrawer_drawString(DisassemblerViewDrawer *__this, const char *s)
        {
            __this->drawString(QString::fromUtf8(s));
        }
    }
}
