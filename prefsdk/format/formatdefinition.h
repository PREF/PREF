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

        public:
            explicit FormatDefinition(const QtLua::LuaTable& formattable, QObject *parent = 0);
            QString id() const;
            bool hasView() const;
            QString name() const;
            QString category() const;
            QString author() const;
            QString version() const;

        public:
            bool callValidate(QHexEditData* hexeditdata, Logger *logger, qint64 baseoffset, bool ignoreerror = false);
            FormatTree* callParse(QHexEditData* hexeditdata, Logger *logger, qint64 baseoffset);
            QWidget* callView(FormatTree* formattree);

        private:
            QtLua::LuaTable _formattable;
    };
}

#endif // PREFSDK_FORMATDEFINITION_H
