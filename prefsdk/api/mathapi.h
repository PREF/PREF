#ifndef MATHAPI_H
#define MATHAPI_H

#include "qhexedit/qhexeditdata.h"
#include "prefsdk/math.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            double Math_entropy(QHexEditData* hexeditdata, uint64_t start, uint64_t size);
        }
    }
}

#endif // MATHAPI_H
