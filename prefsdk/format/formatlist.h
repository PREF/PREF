#ifndef FORMATLIST_H
#define FORMATLIST_H

#include <QtCore>
#include <QApplication>
#include "prefsdk/categorymanager.h"
#include "prefsdk/format/formatdefinition.h"
#include "prefsdk/prefdebug.h"
#include "qhexedit/qhexeditdata.h"

namespace PrefSDK
{
    using namespace Lua;

    class FormatList
    {
        private:
            FormatList();
            static void loadFormats(lua_State* l, QString dir);

        public:
            static void load(lua_State *l);
            static int length();
            static FormatDefinition::Ptr format(int i);

        private:
            static LuaTable::Ptr _fltable;
            static QList<FormatDefinition::Ptr> _formats;
            static const QString FORMATS_DIR;
            static const QString FORMAT_MAIN_FILE;
    };
}

#endif // FORMATLIST_H
