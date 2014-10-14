#ifndef PREFSDK_FORMATDEFINITION_H
#define PREFSDK_FORMATDEFINITION_H

#include "logwidget/logobject.h"
#include "prefsdk/libs/qt/qtlua.h"
#include "prefsdk/format/formattree.h"
#include "formatvalidator.h"
#include "prefsdk/luastate.h"

namespace PrefSDK
{
    class FormatDefinition : public LogObject
    {
        Q_OBJECT

        Q_PROPERTY(QString id READ id)
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(QString category READ category WRITE setCategory)
        Q_PROPERTY(QString author READ author WRITE setAuthor)
        Q_PROPERTY(QString version READ version WRITE setVersion)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction validate READ validate WRITE setValidate)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction parse READ parse WRITE setParse)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction view READ view WRITE setView)

        public:
            explicit FormatDefinition(QObject *parent = 0);
            explicit FormatDefinition(const QString& name, const QString& category, const QString& author, const QString& version, QObject *parent = 0);
            QString id() const;
            bool hasView() const;
            const QString& name() const;
            const QString& category() const;
            const QString& author() const;
            const QString& version() const;
            const PrefSDK::QtLua::LuaFunction& validate() const;
            const PrefSDK::QtLua::LuaFunction& parse() const;
            const PrefSDK::QtLua::LuaFunction& view() const;
            void setName(const QString& s);
            void setCategory(const QString& s);
            void setAuthor(const QString& s);
            void setVersion(const QString& s);
            void setValidate(const PrefSDK::QtLua::LuaFunction& vf);
            void setParse(const PrefSDK::QtLua::LuaFunction& pf);
            void setView(const PrefSDK::QtLua::LuaFunction& vf);

        public:
            bool callValidate(QHexEditData* hexeditdata, Logger *logger, qint64 baseoffset, bool ignoreerror = false);
            FormatTree* callParse(QHexEditData* hexeditdata, Logger *logger, qint64 baseoffset);
            QWidget* callView(QHexEditData* hexeditdata, FormatTree* formattree);

        private:
            PrefSDK::QtLua::LuaFunction _validatefunc;
            PrefSDK::QtLua::LuaFunction _parsefunc;
            PrefSDK::QtLua::LuaFunction _viewfunc;
            QString _name;
            QString _category;
            QString _author;
            QString _version;
    };
}

#endif // PREFSDK_FORMATDEFINITION_H
