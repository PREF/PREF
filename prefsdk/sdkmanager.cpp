#include "sdkmanager.h"

namespace PrefSDK
{
    const char* SDKManager::SDK_TABLE = "Sdk";
    const QString SDKManager::SDK_DIR = "sdk";
    const QString SDKManager::MAIN_SCRIPT = "main.lua";
    QString SDKManager::_sdkpath;
    //PrefLib::SdkVersion SDKManager::_sdkversion = { false, 0, 0, 0, QString() };

    SDKManager::SDKManager()
    {

    }

    void SDKManager::loadMain(lua_State* l, QString sdkpath, QString filename)
    {
        int res = luaL_dofile(l, QString("%1%2%3").arg(sdkpath, QDir::separator(), filename).toLatin1().constData());

        if(res != 0)
        {
            throw std::runtime_error(lua_tostring(l, -1));
            lua_pop(l, 1);
        }
    }

    bool SDKManager::loadSDK()
    {
        QString appdir = qApp->applicationDirPath();
        PrefContext* prefctx = PrefContext::instance();
        prefctx->addSearchPath(appdir.toUtf8().constData());

        QDir d(appdir);
        SDKManager::_sdkpath = d.absoluteFilePath(SDKManager::SDK_DIR);

        if(!QDir(SDKManager::_sdkpath).exists())
            return false;

        /*
        lua_State* l = LuaState::instance();
        luaopen_capstone(l);
        SDKManager::loadMain(l, d.absoluteFilePath(SDKManager::_sdkpath), SDKManager::MAIN_SCRIPT);

        FormatList::load();
        ExporterList::load();
        DisassemblerList::load();

        SQLite::SQLiteDatabase::initialize();
        SignatureDatabase::load();
        */

        return true;
    }

    QString SDKManager::sdkVersion()
    {
        /*
        if(SDKManager::_sdkversion.IsLoaded)
        {
            QString s = QString("%1.%2").arg(QString::number(SDKManager::_sdkversion.Major), QString::number(SDKManager::_sdkversion.Minor));

            if(SDKManager::_sdkversion.Revision > 0)
                s.append(".").append(QString::number(SDKManager::_sdkversion.Revision));

            if(!SDKManager::_sdkversion.Custom.isEmpty())
                s.append(" ").append(SDKManager::_sdkversion.Custom);

            return s;
        }
        */

        //FIXME: sdkVersion

        return "Missing SDK or Invalid Version";
    }

    QString SDKManager::sdkPath()
    {
        return SDKManager::_sdkpath;
    }
}
