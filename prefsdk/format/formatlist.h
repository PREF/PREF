#ifndef FORMATLIST_H
#define FORMATLIST_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/luastate.h"
#include "prefsdk/categorymanager.h"
#include "formatdefinition.h"
#include "formattree.h"

namespace PrefSDK
{
    class FormatList: public QObject
    {
        Q_OBJECT

        private:
            FormatList(QObject* parent = 0);
            void load(lua_State* l, const QString &dir);

        public:
            static FormatList* instance();
            static void load();
            FormatDefinition* format(int i);
            int length();

        private:
            static FormatList* _instance;
            QList<FormatDefinition*> _formats;
            QHash<QString, int> _formatmap;

        private:
            static const QString FORMATS_DIR;
            static const QString FORMAT_MAIN_FILE;
    };
}

#endif // FORMATLIST_H
