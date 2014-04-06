#include "sdkmanager.h"

namespace PrefSDK
{
    /* PREF Tables */
    PrefTable::Ptr SDKManager::_preftable;
    PrefDebug::Ptr SDKManager::_prefdebug;
    PrefUI::Ptr SDKManager::_prefui;

    SDKManager::SdkVersionFunction::Ptr SDKManager::_sdkversion;
    lua_State* SDKManager::_state = nullptr;

    const QString SDKManager::SDK_DIR = "sdk";
    const QString SDKManager::MAIN_SCRIPT = "main.lua";

    SDKManager::SDKManager()
    {
    }

    void SDKManager::loadSdkVersion()
    {
        SDKManager::_sdkversion = SDKManager::SdkVersionFunction::global(SDKManager::_state, "sdkVersion");
    }

    /*
    void SDKManager::loadSdkFiles(const QString& dir)
    {
        QDir d(dir);
        QFileInfoList files = d.entryInfoList(QStringList() << "*.lua", QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

        foreach(QFileInfo fi, files)
        {
            if(fi.isFile())
            {
                try
                {
                    luaW_dofile(SDKManager::_state, fi.absoluteFilePath().toLatin1().constData());
                }
                catch(LuaException& e)
                {
                    PrefDebug::dbgprint(e.what());
                }
            }
            else
                SDKManager::loadSdkFiles(fi.absoluteFilePath());
        }
    }
    */

    void SDKManager::loadPrefTables()
    {
        SDKManager::_preftable = PrefTable::create(SDKManager::_state);
        SDKManager::_prefdebug = PrefDebug::create(SDKManager::_state);
        SDKManager::_prefui = PrefUI::create(SDKManager::_state);

        SDKManager::_preftable->pushGlobal();
        SDKManager::_prefui->pushGlobal();

        //DisassemblerSegment::declareGlobals(SDKManager::_state);
        Operand::declareGlobals(SDKManager::_state);
        Instruction::declareGlobals(SDKManager::_state);
        ReferenceTable::declareGlobals(SDKManager::_state);
        OutputBuffer::declareGlobals(SDKManager::_state);
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

    void SDKManager::runScript(QString sdkpath, QString filename)
    {
        try
        {
            luaW_dofile(SDKManager::_state, QString("%1%2%3").arg(sdkpath, QDir::separator(), filename).toLatin1().constData());
        }
        catch(LuaException& e)
        {
            PrefDebug::dbgprint(e.what());
        }
    }

    lua_State *SDKManager::initializeLua()
    {
        SDKManager::_state = luaW_newstate(true);
        return SDKManager::_state;
    }

    bool SDKManager::loadSDK()
    {
        if(!SDKManager::_state)
            SDKManager::initializeLua();

        luaW_addsearchpath(SDKManager::_state, qApp->applicationDirPath().toLatin1().constData());

        QDir d(qApp->applicationDirPath());
        QString sdkpath = d.absoluteFilePath(SDKManager::SDK_DIR);

        if(!QDir(sdkpath).exists())
            return false;

        try
        {
            SDKManager::runScript(d.absoluteFilePath(sdkpath), SDKManager::MAIN_SCRIPT);
            SDKManager::loadSdkVersion();
            SDKManager::loadPrefTables();

            SQLite::SQLiteDatabase::initialize();
            SignatureDatabase::load();
        }
        catch(LuaException& e)
        {
            DebugDialog::instance()->luaOut(e.what());
            return false;
        }

        return true;
    }

    void SDKManager::registerMessageHandler()
    {
        //qInstallMessageHandler(&SDKManager::prefAtPanic);
        lua_atpanic(SDKManager::_state, &SDKManager::luaAtPanic);
    }

    QString SDKManager::version()
    {
        if(_sdkversion)
            return QString::fromLatin1((*_sdkversion)());

        return "Missing SDK";
    }

    lua_State *SDKManager::state()
    {
        return SDKManager::_state;
    }
}
