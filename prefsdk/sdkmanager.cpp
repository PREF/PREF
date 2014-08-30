#include "sdkmanager.h"

namespace PrefSDK
{
    const char* SDKManager::SDK_TABLE = "Sdk";
    const QString SDKManager::SDK_DIR = "sdk";
    const QString SDKManager::MAIN_SCRIPT = "main.lua";

    SDKManager::SDKManager()
    {

    }

    void SDKManager::loadMain(lua_State* l, QString sdkpath, QString filename)
    {
        int res = luaL_dofile(l, QString("%1%2%3").arg(sdkpath, QDir::separator(), filename).toLatin1().constData());

        if(res != 0)
        {
            DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);
        }
    }

    int SDKManager::luaAtPanic(lua_State* l)
    {
        int top = lua_gettop(l);

        DebugDialog::instance()->luaOut(QString::fromUtf8(lua_tostring(l, top)))->exec();
        return 0;
    }

    lua_State *SDKManager::initializeLua()
    {
        lua_State* l = LuaState::instance();

        if(l)
        {
            luaL_openlibs(l);
            QtLua::open(l);
            PrefLib::open(l);

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
        QString sdkpath = d.absoluteFilePath(SDKManager::SDK_DIR);

        if(!QDir(sdkpath).exists())
            return false;

        lua_State* l = LuaState::instance();
        SDKManager::loadMain(l, d.absoluteFilePath(sdkpath), SDKManager::MAIN_SCRIPT);

        FormatList::load(l);
        ExporterList::load(l);
        LoaderList::load(l);

        SQLite::SQLiteDatabase::initialize();
        SignatureDatabase::load();
        return true;
    }

    void SDKManager::registerMessageHandler()
    {
        lua_atpanic(LuaState::instance(), &SDKManager::luaAtPanic);
    }
}
