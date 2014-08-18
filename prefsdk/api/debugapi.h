#ifndef PREFSDK_API_DEBUGAPI_H
#define PREFSDK_API_DEBUGAPI_H

#include "debugdialog/debugdialog.h"
#include "qhexedit/qhexeditdata.h"
#include "mainwindow.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            void Debug_log(QHexEditData* hexeditdata, const char* s);
            void Debug_logNotice(QHexEditData* hexeditdata, const char* s);
            void Debug_logWarning(QHexEditData* hexeditdata, const char* s);
            void Debug_logError(QHexEditData* hexeditdata, const char* s);
            void Debug_showDialog(const char* s);
        }
    }
}

#endif // PREFSDK_API_DEBUGAPI_H
