#ifndef PREFSDK_API_PROCESSORAPI_H
#define PREFSDK_API_PROCESSORAPI_H

#include "prefsdk/disassembler/disassembler.h"
#include "prefsdk/format/formatlist.h"

namespace PrefSDK
{
    namespace API
    {
        /* DisassemblerSegment's flags */
        extern "C" const uint64_t SegmentType_Data = DisassemblerSegment::Data;
        extern "C" const uint64_t SegmentType_Code = DisassemblerSegment::Code;

        extern "C"
        {
            void ProcessorLoader_addSegment(FormatList::FormatId formatid, const char* name, uint64_t segmenttype, uint64_t startoffset, uint64_t endoffset, uint64_t baseaddress);
            void ProcessorLoader_addEntryPoint(FormatList::FormatId formatid, const char* name, uint64_t offset);
        }
    }
}

#endif // PREFSDK_API_PROCESSORAPI_H
