#ifndef SDKMANAGER_H
#define SDKMANAGER_H

#include <preflib.h>
#include "prefsdk/signatures/signaturedatabase.h"

namespace PrefSDK {

using namespace PrefLib;

class SDKManager
{
    private:
        SDKManager();
        static void executeFile(PrefContext *prefctx, QString filename);
        static void loadScripts(PrefContext *prefctx, QString foldername);

    public:
        static bool loadSDK();
        static void unloadSDK();
        static QString sdkVersion();
        static const QString& sdkPath();

    private:
        static QString _sdkpath;
};

} // namespace PrefSDK;

#endif // SDKMANAGER_H
