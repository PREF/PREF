#include "processorapi.h"

namespace PrefSDK
{
    namespace API
    {
        void ProcessorLoader_addSegment(FormatList::FormatId formatid, const char *name, uint64_t segmenttype, uint64_t startoffset, uint64_t endoffset, uint64_t baseaddress)
        {
            FormatList::Format& format = FormatList::formatFromId(formatid);
            ProcessorLoader& loader = format.disassemblerLoader();

            loader.addSegment(QString::fromUtf8(name), static_cast<DisassemblerSegment::SegmentType>(segmenttype), startoffset, endoffset, baseaddress);
        }

        void ProcessorLoader_addEntryPoint(FormatList::FormatId formatid, const char *name, uint64_t offset)
        {
            FormatList::Format& format = FormatList::formatFromId(formatid);
            ProcessorLoader& loader = format.disassemblerLoader();

            loader.addEntryPoint(QString::fromUtf8(name), offset);
        }
    }
}
