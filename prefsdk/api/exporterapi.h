#ifndef PREFSDK_API_EXPORTERAPI_H
#define PREFSDK_API_EXPORTERAPI_H

#include "prefsdk/exporter/exporterlist.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            void Exporter_register(const char* name, const char* description, const char* author, const char* version, ExporterList::ExporterId exporterid);
        }
    }
}

#endif // PREFSDK_API_EXPORTERAPI_H
