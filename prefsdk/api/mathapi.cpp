#include "mathapi.h"

namespace PrefSDK
{
    namespace API
    {
        double Math_entropy(QHexEditData *hexeditdata, uint64_t start, uint64_t size)
        {
            return entropy(hexeditdata, start, size);
        }
    }
}
