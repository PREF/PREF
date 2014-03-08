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
#include "prefsdk/endian.h"
#include "prefsdk/datatype.h"
#include "prefsdk/prefui.h"

namespace PrefSDK
{
    class SDKManager
    {
        private:
            typedef LuaFunctionT<lua_String> SdkVersionFunction;

        private:
            SDKManager();
            static void runScript(QString sdkpath, QString filename);
            static void loadSdkVersion(QString sdkpath);
            static void loadSdkFiles(const QString &dir);
            static void loadPrefTables();

        public:
            static lua_State* initializeLua();
            static bool loadSDK();
            static void unloadSDK();
            static QString version();
            static lua_State* state();

        private:
            static PrefDebug::Ptr _prefdebug;
            static PrefUI::Ptr _prefui;
            static Endian::Ptr _endian;
            static DataType::Ptr _datatype;

        private:
            static lua_State* _state;
            static SdkVersionFunction::Ptr _sdkversion;
            static const QString SDK_DIR;
            static const QString REQUIRE_SCRIPT;
            static const QString VERSION_SCRIPT;
    };
}

#endif // SDKMANAGER_H
