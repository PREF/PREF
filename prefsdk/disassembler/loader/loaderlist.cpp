#include "loaderlist.h"

namespace PrefSDK
{
    LoaderList* LoaderList::_instance = nullptr;

    const QString LoaderList::LOADERS_DIR = "loaders";
    const QString LoaderList::LOADER_MAIN_FILE = "definition.lua";

    LoaderList::LoaderList(QObject *parent): QObject(parent)
    {
    }

    void LoaderList::load(lua_State *l, const QString &dir)
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
                    throw PrefException("LoaderList::load(): QString::fromUtf8(lua_tostring(l, -1))");
                    lua_pop(l, 1);
                }

                if(!QtLua::isQObject(l, -1))
                {
                    throw PrefException(QString("LoaderList::load(): Got '%1'' instead of loader type").arg(QString::fromUtf8(lua_typename(l, lua_type(l, -1)))));
                    lua_pop(l, 1);
                    continue;
                }

                ProcessorLoader* processorloader = qobject_cast<ProcessorLoader*>(QtLua::toQObject(l, -1));
                lua_pop(l, 1);

                if(this->_loadermap.contains(processorloader->id()))
                {
                    throw PrefException(QString("LoaderList::load(): Loader '%1' already loaded").arg(processorloader->name()));
                    continue;
                }

                int idx = this->_loaders.length();
                this->_loaders.append(processorloader);
                this->_loadermap[processorloader->id()] = idx;
            }
        }
    }

    LoaderList *LoaderList::instance()
    {
        return LoaderList::_instance;
    }

    void LoaderList::load()
    {
        if(LoaderList::_instance)
            return;

        LoaderList::_instance = new LoaderList();
        QDir d(qApp->applicationDirPath());
        QString loaderspath = d.absoluteFilePath(LoaderList::LOADERS_DIR);

        if(!QDir(loaderspath).exists())
            return;

        lua_State* l = LuaState::instance();
        LoaderList::_instance->load(l, loaderspath);
    }

    ProcessorLoader *LoaderList::loader(int i)
    {
        return this->_loaders[i];
    }

    int LoaderList::length()
    {
        return this->_loaders.length();
    }
}
