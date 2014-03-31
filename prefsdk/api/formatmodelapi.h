#ifndef PREFSDK_API_FORMATMODELAPI_H
#define PREFSDK_API_FORMATMODELAPI_H

#include "viewmodels/formatmodel/formatmodel.h"
#include "prefsdk/format/formattree.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C" void FormatModel_setFormatTree(FormatModel *__this, FormatTree* formattree);
    }
}

#endif // FORMATMODELAPI_H
