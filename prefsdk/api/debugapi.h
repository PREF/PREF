#ifndef PREFSDK_API_DEBUGAPI_H
#define PREFSDK_API_DEBUGAPI_H

#include "debugdialog/debugdialog.h"
#include "mainwindow.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            void Debug_print(const char* s);
            void Debug_println(const char* s);
            void Debug_showDialog();
        }
    }
}

#endif // PREFSDK_API_DEBUGAPI_H
