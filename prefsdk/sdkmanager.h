#ifndef SDKMANAGER_H
#define SDKMANAGER_H

#include <QtCore>
#include <QtWidgets>
#include <cstdint>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/signatures/signaturedatabase.h"
#include "prefsdk/qt/qtlua.h"
#include "prefsdk/preflib/preflib.h"
#include "prefsdk/sqlitewrapper/sqlite.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/format/formattree.h"
#include "prefsdk/exporter/exporterlist.h"
#include "prefsdk/disassembler/loader/loaderlist.h"
#include "prefsdk/luastate.h"

namespace PrefSDK
{
    class SDKManager
    {
        private:
            SDKManager();
            static void loadMain(lua_State *l, QString sdkpath, QString filename);

        private:
            static int luaAtPanic(lua_State* l);

        public:
            static lua_State* initializeLua();
            static void registerMessageHandler();
            static bool loadSDK();
            static void unloadSDK();

        private:
            static const char* SDK_TABLE;
            static const QString SDK_DIR;
            static const QString MAIN_SCRIPT;
    };
}

#endif // SDKMANAGER_H
