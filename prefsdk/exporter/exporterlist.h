#ifndef PREFSDK_EXPORTERLIST_H
#define PREFSDK_EXPORTERLIST_H

#include <QtCore>
#include <lua.hpp>
#include "debugdialog/debugdialog.h"
#include "qhexedit/qhexeditdata.h"

namespace PrefSDK
{
    class ExporterList
    {
        public:
            typedef const char* ExporterId;

            class Exporter
            {
                public:
                    Exporter(): _id(nullptr) { }
                    Exporter(const QString& name, const QString& description, const QString& author, const QString& version, ExporterList::ExporterId exporterid): _name(name), _description(description), _author(author), _version(version), _id(exporterid) { }
                    const QString& name() const { return this->_name; }
                    const QString& description() const { return this->_description; }
                    const QString& author() const { return this->_author; }
                    const QString& version() const { return this->_version; }
                    ExporterList::ExporterId id() const { return this->_id; }

                private:
                    QString _name;
                    QString _description;
                    QString _author;
                    QString _version;
                    ExporterList::ExporterId _id;
            };

        private:
            ExporterList();
            static void loadExporters(lua_State* l, QString dir);

        public:
            static void registerExporter(const QString& name, const QString& description, const QString& author, const QString& version, ExporterList::ExporterId exporterid);
            static void exportData(ExporterList::ExporterId exporterid, const QString &filename, QHexEditData* hexeditdatain, quint64 startoffset, quint64 endoffset);
            static const ExporterList::Exporter& exporter(int i);
            static void load(lua_State *l);
            static int length();

        private:
            static QList<ExporterList::Exporter> _exporters;
            static lua_State* _state;
            static const QString EXPORTER_MAIN_FILE;
            static const QString EXPORTERS_DIR;
    };
}

#endif // PREFSDK_EXPORTERLIST_H
