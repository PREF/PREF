#include "loaderlist.h"

namespace PrefSDK
{
    QList<LoaderList::Loader> LoaderList::_loaders;
    QHash<LoaderList::LoaderId, int> LoaderList::_loadermap;

    const QString LoaderList::LOADERS_DIR = "loaders";
    const QString LoaderList::LOADER_MAIN_FILE = "register.lua";

    LoaderList::LoaderList()
    {
    }

    void LoaderList::loadLoader(lua_State *l, const QString &dir)
    {
        QDir d(dir);
        QFileInfoList dirs = d.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

        foreach(QFileInfo fi, dirs)
        {
            QDir fd(fi.absoluteFilePath());

            if(fd.exists(LoaderList::LOADER_MAIN_FILE))
            {
                int res = luaL_dofile(l, fd.absoluteFilePath(LoaderList::LOADER_MAIN_FILE).toUtf8().constData());

                if(res != 0)
                {
                    DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(l, -1)));
                    lua_pop(l, 1);
                }
            }
        }
    }

    void LoaderList::load(lua_State *l)
    {
        QDir d(qApp->applicationDirPath());
        QString formatspath = d.absoluteFilePath(LoaderList::LOADERS_DIR);

        if(!QDir(formatspath).exists())
            return;

        LoaderList::loadLoader(l, formatspath);
    }

    void LoaderList::registerLoader(const QString &name, const QString &author, const QString &version, LoaderList::LoaderId loaderid)
    {
        int idx = LoaderList::_loaders.length();

        LoaderList::_loaders.append(Loader(name, author, version, loaderid));
        LoaderList::_loadermap[loaderid] = idx;
    }

    LoaderList::Loader &LoaderList::loader(LoaderList::LoaderId loaderid)
    {
        return LoaderList::_loaders[LoaderList::_loadermap[loaderid]];
    }
}
