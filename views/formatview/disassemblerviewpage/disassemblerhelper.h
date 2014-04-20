#ifndef DISASSEMBLERHELPER_H
#define DISASSEMBLERHELPER_H

#include <QtCore>
#include <QtGui>
#include <lua.hpp>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/sdkmanager.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/disassembler/addressqueue.h"

using namespace PrefSDK;

class DisassemblerHelper : public QObject
{
    Q_OBJECT

    public:
        explicit DisassemblerHelper(QObject *parent = 0);
        void run(QHexEditData *hexeditdata);
        void stop();

    signals:
        void finished();

    private:
        bool _cancontinue;
};

#endif // DISASSEMBLERHELPER_H
