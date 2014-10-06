#ifndef PREFSDK_EXPORTERDEFINITION_H
#define PREFSDK_EXPORTERDEFINITION_H

#include <QtCore>
#include "prefsdk/libs/qt/qtlua.h"
#include "prefsdk/libs/preflib/luahexeditdata.h"

namespace PrefSDK
{
    class ExporterDefinition : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QString id READ id)
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(QString description READ description WRITE setDescription)
        Q_PROPERTY(QString author READ author WRITE setAuthor)
        Q_PROPERTY(QString version READ version WRITE setVersion)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction dump READ dump WRITE setDump)

        public:
            explicit ExporterDefinition(QObject *parent = 0);
            explicit ExporterDefinition(const QString& name, const QString& description, const QString& author, const QString& version, QObject *parent = 0);
            QString id();
            const QString& name() const;
            const QString& description() const;
            const QString& author() const;
            const QString& version() const;
            const PrefSDK::QtLua::LuaFunction& dump() const;
            void setName(const QString& s);
            void setDescription(const QString& s);
            void setAuthor(const QString& s);
            void setVersion(const QString& s);
            void setDump(const PrefSDK::QtLua::LuaFunction &df);

        public:
            void callDump(QHexEditData* hexeditdatain, const QString &filename, qint64 startoffset, qint64 endoffset);

        private:
            PrefSDK::QtLua::LuaFunction _dumpfunction;
            QString _name;
            QString _description;
            QString _author;
            QString _version;
    };
}

#endif // PREFSDK_EXPORTERDEFINITION_H
