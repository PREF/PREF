#include "sdkmanager.h"

namespace PrefSDK
{
    SDKManager::SdkVersion SDKManager::_sdkversion = {false, 0, 0, 0, QString()};
    lua_State* SDKManager::_state = nullptr;

    const char* SDKManager::SDK_TABLE = "Sdk";
    const QString SDKManager::SDK_DIR = "sdk";
    const QString SDKManager::MAIN_SCRIPT = "main.lua";

    SDKManager::SDKManager()
    {

    }

    void SDKManager::loadMain(QString sdkpath, QString filename)
    {
        int res = luaL_dofile(SDKManager::_state, QString("%1%2%3").arg(sdkpath, QDir::separator(), filename).toLatin1().constData());

        if(res != 0)
        {
            DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(SDKManager::_state, -1)));
            lua_pop(SDKManager::_state, 1);
        }
    }

    int SDKManager::luaAtPanic(lua_State* l)
    {
        int top = lua_gettop(l);

        DebugDialog::instance()->luaOut(QString::fromUtf8(lua_tostring(l, top)))->exec();
        return 0;
    }

    void SDKManager::prefAtPanic(QtMsgType msgtype, const QMessageLogContext& context, const QString& msg)
    {
        QString endmsg;

        switch(msgtype)
        {
            case QtDebugMsg:
                endmsg.append("DEBUG: ");
                break;

            case QtWarningMsg:
                endmsg.append("WARNING: ");
                break;

            case QtCriticalMsg:
                endmsg.append("CRITICAL: ");
                break;

            case QtFatalMsg:
                endmsg.append("FATAL: ");
                break;

            default:
                break;
        }

        endmsg.append(QString("%1 (%2:%3, %4)").arg(msg, QString::fromUtf8(context.file), QString::number(context.line), QString::fromUtf8(context.function)));
        DebugDialog::instance()->out(endmsg);
    }

    lua_State *SDKManager::initializeLua()
    {
        SDKManager::_state = luaL_newstate();

        if(SDKManager::_state)
        {
            luaL_openlibs(SDKManager::_state);

            lua_getglobal(SDKManager::_state, "package");
            lua_getfield(SDKManager::_state, -1, "path");

            const char* origpath = lua_tostring(SDKManager::_state, -1);
            lua_pop(SDKManager::_state, 1); /* Pop 'path' */

            QString newpath = QString("%1;%2/?.lua").arg(QString::fromUtf8(origpath), qApp->applicationDirPath());
            lua_pushstring(SDKManager::_state, newpath.toUtf8().constData());
            lua_setfield(SDKManager::_state, -2, "path");
            lua_pop(SDKManager::_state, 1); /* Pop 'package' */
        }

        return SDKManager::_state;
    }

    void SDKManager::setVersion(int8_t major, int8_t minor, int8_t revision, const QString &custom)
    {
        SDKManager::_sdkversion = {true, major, minor, revision, custom};
    }

    bool SDKManager::loadSDK()
    {
        QDir d(qApp->applicationDirPath());
        QString sdkpath = d.absoluteFilePath(SDKManager::SDK_DIR);

        if(!QDir(sdkpath).exists())
            return false;

        SDKManager::loadMain(d.absoluteFilePath(sdkpath), SDKManager::MAIN_SCRIPT);

        FormatList::load(SDKManager::_state);
        ExporterList::load(SDKManager::_state);

        SQLite::SQLiteDatabase::initialize();
        SignatureDatabase::load();
        return true;
    }

    void SDKManager::registerMessageHandler()
    {
        qInstallMessageHandler(&SDKManager::prefAtPanic);
        lua_atpanic(SDKManager::_state, &SDKManager::luaAtPanic);
    }

    QString SDKManager::version()
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

        return "Missing SDK";
    }

    lua_State *SDKManager::state()
    {
        return SDKManager::_state;
    }

    FormatTree *SDKManager::parseFormat(FormatList::FormatId formatid, int64_t baseoffset, QHexEditData* hexeditdata)
    {
        FormatTree* formattree = new FormatTree(SDKManager::_state, hexeditdata, baseoffset);

        lua_getglobal(SDKManager::_state, SDKManager::SDK_TABLE);
        lua_getfield(SDKManager::_state, -1, "parseFormat");
        lua_pushstring(SDKManager::_state, formatid);
        lua_pushinteger(SDKManager::_state, static_cast<lua_Integer>(baseoffset));
        lua_pushlightuserdata(SDKManager::_state, hexeditdata);
        lua_pushlightuserdata(SDKManager::_state, formattree);
        int res = lua_pcall(SDKManager::_state, 4, 0, 0);

        if(res != 0)
            DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(SDKManager::_state, -1)));

        lua_pop(SDKManager::_state, (res ? 2 : 1));
        return formattree;
    }
}
