#include "sdkmanager.h"

namespace PrefSDK {

QString SDKManager::_sdkpath;

SDKManager::SDKManager()
{

}

void SDKManager::executeFile(PrefContext* prefctx, QString filename)
{
    QString completepath = QString("%1%2%3").arg(SDKManager::_sdkpath, QDir::separator(), filename);
    prefctx->executeScript(completepath.toUtf8().constData());
}

void SDKManager::loadScripts(PrefContext* prefctx, QString foldername)
{
    QDir d(QString("%1%2%3").arg(qApp->applicationDirPath(), QDir::separator(), foldername));
    QFileInfoList dirs = d.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    foreach(QFileInfo fi, dirs)
    {
        QDir fd(fi.absoluteFilePath());

        if(fd.exists("definition.lua"))
            prefctx->executeScript(fd.absoluteFilePath("definition.lua").toUtf8().constData());
    }
}

bool SDKManager::loadSDK()
{
    QString appdir = qApp->applicationDirPath();
    PrefContext* prefctx = PrefContext::instance();
    prefctx->addSearchPath(appdir.toUtf8().constData());

    QDir d(appdir);
    SDKManager::_sdkpath = d.absoluteFilePath("sdk");

    if(!QDir(SDKManager::_sdkpath).exists())
        return false;

    SDKManager::executeFile(prefctx, "main.lua");
    SDKManager::loadScripts(prefctx, "exporters");
    SDKManager::loadScripts(prefctx, "formats");

    /* TODO:
    DisassemblerList::load();

    SQLite::SQLiteDatabase::initialize();
    SignatureDatabase::load();
    */

    return true;
}

QString SDKManager::sdkVersion()
{
    return "2.0";
}

const QString &SDKManager::sdkPath()
{
    return SDKManager::_sdkpath;
}

} // namespace PrefSDK
