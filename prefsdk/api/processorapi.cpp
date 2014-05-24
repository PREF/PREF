#include "processorapi.h"

namespace PrefSDK
{
    namespace API
    {
        void Loader_register(const char *name, const char *author, const char *version, LoaderList::LoaderId loaderid)
        {
            LoaderList::registerLoader(QString::fromUtf8(name), QString::fromUtf8(author), QString::fromUtf8(version), loaderid);
        }

        void Loader_addSegment(QHexEditData *hexeditdata, const char* name, int64_t startaddress, int64_t endaddress, int64_t baseaddress)
        {
            LoaderList::LoadedLoader& ll = LoaderList::loadedLoader(hexeditdata);
            ll.addSegment(QString::fromUtf8(name), startaddress, endaddress, baseaddress);
        }

        void LoaderModel_setValid(LoaderListModel *__this, LoaderList::LoaderId loaderid)
        {
            __this->setValid(loaderid);
        }

        void DisassemblerDrawer_drawVirtualAddress(DisassemblerDrawer *__this, const char *segmentname, const char *address)
        {
            __this->drawVirtualAddress(QString::fromUtf8(segmentname), QString::fromUtf8(address));
        }

        void DisassemblerDrawer_drawHexDump(DisassemblerDrawer *__this, uint64_t offset, int dumplength, int maxwidth)
        {
            __this->drawHexDump(offset, dumplength, maxwidth);
        }

        void DisassemblerDrawer_drawMnemonic(DisassemblerDrawer *__this, int width, const char *mnemonic, int instructionfeatures)
        {
            __this->drawMnemonic(width, QString::fromUtf8(mnemonic), static_cast<InstructionFeatures::Features>(instructionfeatures));
        }

        void DisassemblerDrawer_drawString(DisassemblerDrawer *__this, const char *s)
        {
            __this->drawString(QString::fromUtf8(s));
        }

        void DisassemblerDrawer_drawImmediate(DisassemblerDrawer *__this, const char *s)
        {
            __this->drawImmediate(QString::fromUtf8(s));
        }

        void DisassemblerDrawer_drawAddress(DisassemblerDrawer *__this, const char *s)
        {
            __this->drawAddress(QString::fromUtf8(s));
        }

        void DisassemblerDrawer_drawRegister(DisassemblerDrawer *__this, const char *s)
        {
            __this->drawRegister(QString::fromUtf8(s));
        }
    }
}
