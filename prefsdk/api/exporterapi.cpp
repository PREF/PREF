#include "exporterapi.h"

namespace PrefSDK
{
    namespace API
    {
        void Exporter_register(const char *name, const char *description, const char *author, const char *version, ExporterList::ExporterId exporterid)
        {
            ExporterList::registerExporter(QString::fromUtf8(name), QString::fromUtf8(description), QString::fromUtf8(author), QString::fromUtf8(version), exporterid);
        }
    }
}
