#include "exportlist.h"

namespace PrefSDK
{
    LuaTable::Ptr ExportList::_eltable;
    QList<ExportDefinition::Ptr> ExportList::_exporters;
    const QString ExportList::EXPORTERS_DIR = "exporters";
    const QString ExportList::EXPORTER_MAIN_FILE = "main.lua";

    ExportList::ExportList()
    {
    }

    void ExportList::loadExporters(lua_State *l, QString dir)
    {
        QDir d(dir);
        QFileInfoList dirs = d.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

        foreach(QFileInfo fi, dirs)
        {
            try
            {
                QDir fd(fi.absoluteFilePath());

                if(fd.exists(ExportList::EXPORTER_MAIN_FILE))
                    luaW_dofile(l, fd.absoluteFilePath(ExportList::EXPORTER_MAIN_FILE).toLatin1().constData());
            }
            catch(LuaException& e)
            {
                PrefDebug::dbgprint(e.what());
            }
        }
    }

    void ExportList::load(lua_State *l)
    {
        ExportList::_eltable = LuaTable::create(l);
        ExportList::_eltable->pushGlobal("ExportList");

        ExportList::_exporters.clear();

        QDir d(qApp->applicationDirPath());
        QString exporterspath = d.absoluteFilePath(ExportList::EXPORTERS_DIR);

        if(!QDir(exporterspath).exists())
            return;

        ExportList::loadExporters(l, exporterspath);

        for(lua_Integer i = 0; i < ExportList::_eltable->length(); i++)
        {
            LuaTable::Ptr edt = ExportList::_eltable->get<lua_Integer>(i + 1);
            ExportDefinition::Ptr ed(new ExportDefinition(edt));
            ExportList::_exporters.append(ed);
        }
    }

    int ExportList::length()
    {
        return ExportList::_eltable->length();
    }

    ExportDefinition::Ptr ExportList::exporter(int i)
    {
        return ExportList::_exporters.at(i);
    }
}
