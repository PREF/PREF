#ifndef DISASSEMBLERWORKER_H
#define DISASSEMBLERWORKER_H

#include <QThread>
#include <QLabel>
#include "logwidget/logger.h"
#include "qhexedit/qhexeditdata.h"
#include <disassembler/disassemblerdefinition.h>

using namespace PrefLib::Disassembler;

class DisassemblerWorker : public QThread
{
    Q_OBJECT

    public:
        explicit DisassemblerWorker(QHexEditData* hexeditdata, DisassemblerDefinition* disassembler, LogWidget* logwidget, QLabel* infolabel, const QString& loadedfile, bool canloaddatabase, QObject *parent = 0);

    protected:
        virtual void run();

    private:
        QHexEditData* _hexeditdata;
        DisassemblerDefinition* _disassembler;
        LogWidget* _logwidget;
        QLabel* _infolabel;
        const QString& _loadedfile;
        bool _canloaddatabase;
};

#endif // DISASSEMBLERWORKER_H
