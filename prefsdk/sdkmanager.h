#ifndef SDKMANAGER_H
#define SDKMANAGER_H

#include <QtCore>
#include <QtWidgets>
#include "prefsdk/format/formatlist.h"
#include "prefsdk/signatures/signaturedatabase.h"
#include "prefsdk/disassembler/disassembler.h"
#include "prefsdk/sqlitewrapper/sqlite.h"
#include "prefsdk/qlua.h"
#include "prefsdk/prefdebug.h"
#include "prefsdk/byteorder.h"
#include "prefsdk/prefui.h"
#include "prefsdk/global/preftable.h"

namespace PrefSDK
{
    class SDKManager
    {
        private:
            typedef LuaFunctionT<lua_String> SdkVersionFunction;

        private:
            SDKManager();
            static void runScript(QString sdkpath, QString filename);
            static void loadSdkVersion();
            //static void loadSdkFiles(const QString &dir);
            static void loadPrefTables();
            static int atPanic(lua_State*);

        public:
            static lua_State* initializeLua();
            static bool loadSDK();
            static void unloadSDK();
            static QString version();
            static lua_State* state();

        private:
            static PrefTable::Ptr _preftable;
            static PrefDebug::Ptr _prefdebug;
            static PrefUI::Ptr _prefui;

        private:
            static lua_State* _state;
            static SdkVersionFunction::Ptr _sdkversion;
            static const QString SDK_DIR;
            static const QString MAIN_SCRIPT;
    };
}

#endif // SDKMANAGER_H
