#include "exporterlist.h"

namespace PrefSDK
{
    ExporterList* ExporterList::_instance = nullptr;
    const QString ExporterList::EXPORTER_MAIN_FILE = "definition.lua";
    const QString ExporterList::EXPORTERS_DIR = "exporters";

    ExporterList::ExporterList(QObject *parent): QObject(parent)
    {

    }

    ExporterList *ExporterList::instance()
    {
        return ExporterList::_instance;
    }

    void ExporterList::load()
    {
        if(ExporterList::_instance)
            return;

        ExporterList::_instance = new ExporterList();

        QDir d(qApp->applicationDirPath());
        QString exporterspath = d.absoluteFilePath(ExporterList::EXPORTERS_DIR);

        if(!QDir(exporterspath).exists())
            return;

        lua_State* l = LuaState::instance();
        ExporterList::_instance->load(l, exporterspath);
    }

    void ExporterList::load(lua_State *l, QString dir)
    {
        QDir d(dir);
        QFileInfoList dirs = d.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

        foreach(QFileInfo fi, dirs)
        {
            QDir fd(fi.absoluteFilePath());

            if(fd.exists(ExporterList::EXPORTER_MAIN_FILE))
            {
                int res = luaL_dofile(l, fd.absoluteFilePath(ExporterList::EXPORTER_MAIN_FILE).toUtf8().constData());

                if(res != 0)
                {
                    throw PrefException(QString("ExporterList::load(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
                    lua_pop(l, 1);
                }

                if(lua_type(l, -1) != LUA_TTABLE)
                {
                    throw PrefException(QString("ExporterList::load(): Got '%1'' instead of exporter table").arg(QString::fromUtf8(lua_typename(l, lua_type(l, -1)))));
                    lua_pop(l, 1);
                    continue;
                }

                ExporterDefinition* exporterdefinition = new ExporterDefinition(QtLua::LuaTable(l, -1));
                lua_pop(l, 1);

                if(this->_exportermap.contains(exporterdefinition->id()))
                {
                    throw PrefException(QString("ExporterList::load(): Exporter '%1' already loaded").arg(exporterdefinition->name()));
                    continue;
                }

                int idx = this->_exporters.length();
                this->_exporters.append(exporterdefinition);
                this->_exportermap[exporterdefinition->id()] = idx;
            }
        }
    }

    int ExporterList::length()
    {
        return ExporterList::_exporters.length();
    }

    ExporterDefinition *ExporterList::exporter(int i)
    {
        return this->_exporters[i];
    }
}
