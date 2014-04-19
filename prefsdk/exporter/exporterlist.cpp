#include "exporterlist.h"

namespace PrefSDK
{
    QList<ExporterList::Exporter> ExporterList::_exporters;
    lua_State* ExporterList::_state = nullptr;

    const QString ExporterList::EXPORTER_MAIN_FILE = "main.lua";
    const QString ExporterList::EXPORTERS_DIR = "exporters";

    ExporterList::ExporterList()
    {
    }

    void ExporterList::loadExporters(lua_State *l, QString dir)
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
                    DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(l, -1)));
                    lua_pop(l, 1);
                }
            }
        }
    }

    void ExporterList::load(lua_State *l)
    {
        ExporterList::_state = l;

        QDir d(qApp->applicationDirPath());
        QString exporterspath = d.absoluteFilePath(ExporterList::EXPORTERS_DIR);

        if(!QDir(exporterspath).exists())
            return;

        ExporterList::loadExporters(l, exporterspath);
    }

    void ExporterList::registerExporter(const QString &name, const QString &description, const QString &author, const QString &version, ExporterList::ExporterId exporterid)
    {
        ExporterList::_exporters.append(Exporter(name, description, author, version, exporterid));
    }

    void ExporterList::exportData(ExporterList::ExporterId exporterid, const QString& filename,  QHexEditData *hexeditdatain, quint64 startoffset, quint64 endoffset)
    {
        QHexEditData* hexeditdataout = QHexEditData::fromMemory(QByteArray());

        lua_getglobal(ExporterList::_state, "Sdk");
        lua_getfield(ExporterList::_state, -1, "exportData");
        lua_pushstring(ExporterList::_state, exporterid);
        lua_pushlightuserdata(ExporterList::_state, hexeditdatain);
        lua_pushlightuserdata(ExporterList::_state, hexeditdataout);
        lua_pushinteger(ExporterList::_state, startoffset);
        lua_pushinteger(ExporterList::_state, endoffset);

        int res = lua_pcall(ExporterList::_state, 5, 0, 0);

        if(res != 0)
            DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(ExporterList::_state, -1)));

        lua_pop(ExporterList::_state, (res ? 2 : 1));

        QFile f(filename);
        f.open(QIODevice::WriteOnly | QIODevice::Truncate);
        hexeditdataout->saveTo(&f);
        f.close();
    }

    int ExporterList::length()
    {
        return ExporterList::_exporters.length();
    }

    const ExporterList::Exporter& ExporterList::exporter(int i)
    {
        return ExporterList::_exporters.at(i);
    }
}
