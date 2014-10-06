#ifndef PREFSDK_EXPORTERLIST_H
#define PREFSDK_EXPORTERLIST_H

#include <QtCore>
#include "prefsdk/luastate.h"
#include "qhexedit/qhexeditdata.h"
#include "exporterdefinition.h"

namespace PrefSDK
{
    class ExporterList: public QObject
    {
        Q_OBJECT

        private:
            ExporterList(QObject *parent = 0);
            void load(lua_State* l, QString dir);

        public:
            static ExporterList* instance();
            static void load();
            ExporterDefinition* exporter(int i);
            int length();

        private:
            static ExporterList* _instance;
            QList<ExporterDefinition*> _exporters;
            QHash<QString, int> _exportermap;

        private:
            static const QString EXPORTER_MAIN_FILE;
            static const QString EXPORTERS_DIR;
    };
}

#endif // PREFSDK_EXPORTERLIST_H
