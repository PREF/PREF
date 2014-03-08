#ifndef PREFSDK_EXPORTLIST_H
#define PREFSDK_EXPORTLIST_H

#include "prefsdk/qlua.h"
#include "prefsdk/exporter/exportdefinition.h"
#include "prefsdk/prefdebug.h"

namespace PrefSDK
{
    class ExportList
    {
        private:
            ExportList();
            static void loadExporters(lua_State* l, QString dir);

        public:
            static void load(lua_State *l);
            static int length();
            static ExportDefinition::Ptr exporter(int i);

        private:
            static LuaTable::Ptr _eltable;
            static QList<ExportDefinition::Ptr> _exporters;
            static const QString EXPORTERS_DIR;
            static const QString EXPORTER_MAIN_FILE;
    };
}

#endif // PREFSDK_EXPORTLIST_H
