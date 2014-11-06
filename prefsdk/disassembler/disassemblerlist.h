#ifndef PREFSDK_DISASSEMBLERLIST_H
#define PREFSDK_DISASSEMBLERLIST_H

#include <QtCore>
#include <algorithm>
#include "prefsdk/luastate.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/disassembler/blocks/segment.h"
#include "prefsdk/disassembler/disassemblerdefinition.h"

namespace PrefSDK
{
    class DisassemblerList: public QObject
    {
        Q_OBJECT

        private:
            DisassemblerList(QObject* parent = 0);
            void load(lua_State *l, const QString& dir);

        public:
            static DisassemblerList* instance();
            static void load();
            DisassemblerDefinition* disassembler(int i);
            int length();

        private:
            static DisassemblerList* _instance;
            QList<DisassemblerDefinition*> _disassemblers;
            QHash<QString, int> _disassemblermap;

        private:
            static const QString DISASSEMBLERS_DIR;
            static const QString DISASSEMBLER_MAIN_FILE;
    };
}
#endif // PREFSDK_DISASSEMBLERLIST_H
