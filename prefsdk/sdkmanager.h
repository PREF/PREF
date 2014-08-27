#ifndef SDKMANAGER_H
#define SDKMANAGER_H

#include <QtCore>
#include <QtWidgets>
#include <cstdint>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/signatures/signaturedatabase.h"
#include "prefsdk/qt/qtlua.h"
#include "prefsdk/sqlitewrapper/sqlite.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/format/formattree.h"
#include "prefsdk/exporter/exporterlist.h"
#include "prefsdk/disassembler/loader/loaderlist.h"

namespace PrefSDK
{
    class SDKManager
    {
        private:
            struct SdkVersion
            {
                bool IsLoaded;
                int8_t Major;
                int8_t Minor;
                int8_t Revision;
                QString Custom;
            };

        private:
            SDKManager();
            static void loadMain(QString sdkpath, QString filename);

        private:
            static int luaAtPanic(lua_State* l);

        public:
            static lua_State* initializeLua();
            static void setVersion(int8_t major, int8_t minor, int8_t revision, const QString& custom);
            static void registerMessageHandler();
            static bool loadSDK();
            static void unloadSDK();
            static QString version();
            static lua_State* state();

        private:
            static SdkVersion _sdkversion;

        private:
            static lua_State* _state;
            static const char* SDK_TABLE;
            static const QString SDK_DIR;
            static const QString MAIN_SCRIPT;
    };
}

#endif // SDKMANAGER_H
