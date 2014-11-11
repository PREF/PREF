#ifndef PREFSDK_EXPORTERDEFINITION_H
#define PREFSDK_EXPORTERDEFINITION_H

#include <QtCore>
#include "prefsdk/libs/qt/qtlua.h"
#include "prefsdk/libs/preflib/databuffer.h"

namespace PrefSDK
{
    class ExporterDefinition : public QObject
    {
        Q_OBJECT

        public:
            explicit ExporterDefinition(const QtLua::LuaTable& exportertable, QObject *parent = 0);
            QString id();
            QString name() const;
            QString description() const;
            QString author() const;
            QString version() const;

        public:
            void callDump(QHexEditData* hexeditdatain, const QString &filename, qint64 startoffset, qint64 endoffset);

        private:
            QtLua::LuaTable _exportertable;
    };
}

#endif // PREFSDK_EXPORTERDEFINITION_H
