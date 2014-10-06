#ifndef PREFSDK_LOADERLIST_H
#define PREFSDK_LOADERLIST_H

#include <QtCore>
#include <algorithm>
#include "prefsdk/luastate.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/disassembler/blocks/segment.h"
#include "prefsdk/disassembler/processor/processorloader.h"

namespace PrefSDK
{
    class LoaderList: public QObject
    {
        Q_OBJECT

        private:
            LoaderList(QObject* parent = 0);
            void load(lua_State *l, const QString& dir);

        public:
            static LoaderList* instance();
            static void load();
            ProcessorLoader* loader(int i);
            int length();

        private:
            static LoaderList* _instance;
            QList<ProcessorLoader*> _loaders;
            QHash<QString, int> _loadermap;

        private:
            static const QString LOADERS_DIR;
            static const QString LOADER_MAIN_FILE;
    };
}
#endif // PREFSDK_LOADERLIST_H
