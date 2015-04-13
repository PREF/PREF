#ifndef SDKMANAGER_H
#define SDKMANAGER_H

#include <QtCore>
#include <QtWidgets>
#include <cstdint>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/signatures/signaturedatabase.h"
#include <preflib.h>

namespace PrefSDK
{
    using namespace PrefLib;

    class SDKManager
    {
        private:
            SDKManager();
            static void loadMain(lua_State *l, QString sdkpath, QString filename);

        private:
            static int luaAtPanic(lua_State* l);

        public:
            static bool loadSDK();
            static void unloadSDK();
            static QString sdkVersion();
            static QString sdkPath();

        private:
            static const char* SDK_TABLE;
            static const QString SDK_DIR;
            static const QString MAIN_SCRIPT;
            static QString _sdkpath;
            //static PrefLib::SdkVersion _sdkversion;
    };
}

#endif // SDKMANAGER_H
