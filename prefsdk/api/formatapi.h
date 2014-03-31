#ifndef PREFSDK_API_FORMATAPI_H
#define PREFSDK_API_FORMATAPI_H

#include "prefsdk/format/formatlist.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C" void Format_register(const FormatDefinition *formatdefinition);
    }
}

#endif // PREFSDK_API_FORMATAPI_H
