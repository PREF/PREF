#ifndef DISASSEMBLERWORKER_H
#define DISASSEMBLERWORKER_H

#include <QThread>
#include <QLabel>
#include "logwidget/logger.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/disassembler/disassemblerdefinition.h"
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "prefsdk/disassembler/disassemblerdatabase.h"

using namespace PrefSDK;

class DisassemblerWorker : public QThread
{
    Q_OBJECT

    public:
        explicit DisassemblerWorker(QHexEditData* hexeditdata, DisassemblerDefinition* disassembler, LogWidget* logwidget, QLabel* infolabel, const QString& loadedfile, QObject *parent = 0);

    protected:
        virtual void run();

    private:
        QHexEditData* _hexeditdata;
        DisassemblerDefinition* _disassembler;
        LogWidget* _logwidget;
        QLabel* _infolabel;
        const QString& _loadedfile;
};

#endif // DISASSEMBLERWORKER_H
