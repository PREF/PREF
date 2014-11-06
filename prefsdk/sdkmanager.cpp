#include "sdkmanager.h"

namespace PrefSDK
{
    const char* SDKManager::SDK_TABLE = "Sdk";
    const QString SDKManager::SDK_DIR = "sdk";
    const QString SDKManager::MAIN_SCRIPT = "main.lua";
    QString SDKManager::_sdkpath;
    PrefLib::SdkVersion SDKManager::_sdkversion = { false, 0, 0, 0, QString() };

    SDKManager::SDKManager()
    {

    }

    void SDKManager::loadMain(lua_State* l, QString sdkpath, QString filename)
    {
        int res = luaL_dofile(l, QString("%1%2%3").arg(sdkpath, QDir::separator(), filename).toLatin1().constData());

        if(res != 0)
        {
            throw PrefException(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);
        }
    }

    lua_State *SDKManager::initializeLua()
    {
        lua_State* l = LuaState::instance();

        if(l)
        {
            luaL_openlibs(l);
            LuaOOP::open(l);
            QtLua::open(l);
            PrefLib::open(l, &SDKManager::_sdkversion);

            lua_getglobal(l, "package");
            lua_getfield(l, -1, "path");

            const char* origpath = lua_tostring(l, -1);
            lua_pop(l, 1); /* Pop 'path' */

            QString newpath = QString("%1;%2/?.lua").arg(QString::fromUtf8(origpath), qApp->applicationDirPath());
            lua_pushstring(l, newpath.toUtf8().constData());
            lua_setfield(l, -2, "path");
            lua_pop(l, 1); /* Pop 'package' */
        }

        return l;
    }

    bool SDKManager::loadSDK()
    {
        QDir d(qApp->applicationDirPath());
        SDKManager::_sdkpath = d.absoluteFilePath(SDKManager::SDK_DIR);

        if(!QDir(SDKManager::_sdkpath).exists())
            return false;

        lua_State* l = LuaState::instance();
        SDKManager::loadMain(l, d.absoluteFilePath(SDKManager::_sdkpath), SDKManager::MAIN_SCRIPT);

        FormatList::load();
        ExporterList::load();
        DisassemblerList::load();

        SQLite::SQLiteDatabase::initialize();
        SignatureDatabase::load();
        return true;
    }

    QString SDKManager::sdkVersion()
    {
        if(SDKManager::_sdkversion.IsLoaded)
        {
            QString s = QString("%1.%2").arg(QString::number(SDKManager::_sdkversion.Major), QString::number(SDKManager::_sdkversion.Minor));

            if(SDKManager::_sdkversion.Revision > 0)
                s.append(".").append(QString::number(SDKManager::_sdkversion.Revision));

            if(!SDKManager::_sdkversion.Custom.isEmpty())
                s.append(" ").append(SDKManager::_sdkversion.Custom);

            return s;
        }

        return "Missing SDK or Invalid Version";
    }

    QString SDKManager::sdkPath()
    {
        return SDKManager::_sdkpath;
    }
}
