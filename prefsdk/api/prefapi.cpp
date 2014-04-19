#include "prefapi.h"

namespace PrefSDK
{
    namespace API
    {
        void Pref_setSdkVersion(int8_t major, int8_t minor, int8_t revision, const char *custom)
        {
            SDKManager::setVersion(major, minor, revision, (custom ? QString::fromUtf8(custom) : QString()));
        }
    }
}
