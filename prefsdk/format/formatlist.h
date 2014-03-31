#ifndef FORMATLIST_H
#define FORMATLIST_H

#include <QtCore>
#include <QApplication>
#include "prefsdk/categorymanager.h"
#include "viewmodels/formatmodel/formatmodel.h"
#include "prefsdk/prefdebug.h"
#include "qhexedit/qhexeditdata.h"
#include "formatdefinition.h"

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
            static void registerFormat(const FormatDefinition* formatdefinition);
            static int length();
            static const FormatDefinition* format(int i);

        private:
            static QList<const FormatDefinition*> _formats;
            static int _formatscount;
            static const QString FORMATS_DIR;
            static const QString FORMAT_MAIN_FILE;
    };
}

#endif // FORMATLIST_H
