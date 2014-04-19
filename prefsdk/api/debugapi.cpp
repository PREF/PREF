#include "debugapi.h"

namespace PrefSDK
{
    namespace API
    {
        void Debug_print(const char *s)
        {
            DebugDialog::instance()->luaOut(QString::fromUtf8(s));
        }

        void Debug_showDialog()
        {
            if(!DebugDialog::instance()->isVisible())
                DebugDialog::instance()->exec();
        }

        void Debug_println(const char *s)
        {
            DebugDialog::instance()->luaOut(QString::fromUtf8(s).append("\n"));
        }
    }
}
