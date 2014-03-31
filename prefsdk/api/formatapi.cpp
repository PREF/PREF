#include "formatapi.h"

namespace PrefSDK
{
    namespace API
    {
        void Format_register(const FormatDefinition *formatdefinition)
        {
            FormatList::registerFormat(formatdefinition);
        }
    }
}
