#include "sdkmanager.h"

namespace PrefSDK
{
    /* PREF Tables */
    PrefDebug::Ptr SDKManager::_prefdebug;
    PrefUI::Ptr SDKManager::_prefui;
    Endian::Ptr SDKManager::_endian;
    DataType::Ptr SDKManager::_datatype;

    SDKManager::SdkVersionFunction::Ptr SDKManager::_sdkversion;
    lua_State* SDKManager::_state = NULL;

    const QString SDKManager::SDK_DIR = "sdk";
    const QString SDKManager::VERSION_SCRIPT = "version.lua";

    SDKManager::SDKManager()
    {
    }

    void SDKManager::loadSdkVersion(QString sdkpath)
    {
        SDKManager::runScript(sdkpath, SDKManager::VERSION_SCRIPT);
        SDKManager::_sdkversion = SDKManager::SdkVersionFunction::global(SDKManager::_state, "sdkVersion");
    }

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

    void SDKManager::loadPrefTables()
    {
        SDKManager::_prefdebug = PrefDebug::create(SDKManager::_state);
        SDKManager::_prefui = PrefUI::create(SDKManager::_state);
        SDKManager::_endian = Endian::create(SDKManager::_state);
        SDKManager::_datatype = DataType::create(SDKManager::_state);

        SDKManager::_prefui->pushGlobal();
        SDKManager::_endian->pushGlobal();
        SDKManager::_datatype->pushGlobal();

        DisassemblerSegment::declareGlobals(SDKManager::_state);
        Operand::declareGlobals(SDKManager::_state);
        Instruction::declareGlobals(SDKManager::_state);
        ReferenceTable::declareGlobals(SDKManager::_state);
        OutputBuffer::declareGlobals(SDKManager::_state);
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
            SDKManager::loadSdkVersion(d.absoluteFilePath(sdkpath));
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
