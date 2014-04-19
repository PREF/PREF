#ifndef PREFSDK_PREFAPI_H
#define PREFSDK_PREFAPI_H

#include "qhexedit/qhexeditdata.h"
#include "prefsdk/sdkmanager.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            void Pref_setSdkVersion(int8_t major, int8_t minor, int8_t revision, const char* custom);
        }
    }
}

#endif // PREFSDK_PREFAPI_H
