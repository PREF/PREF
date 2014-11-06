#ifndef DISASSEMBLERWORKER_H
#define DISASSEMBLERWORKER_H

#include <QThread>
#include <QLabel>
#include "logwidget/logger.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/disassembler/disassemblerdefinition.h"
#include "prefsdk/disassembler/disassemblerlisting.h"

using namespace PrefSDK;

class DisassemblerWorker : public QThread
{
    Q_OBJECT

    public:
        explicit DisassemblerWorker(QHexEditData* hexeditdata, DisassemblerDefinition* disassemblerdefinition, bool elaborateinstructions, bool analyzelisting, LogWidget* logwidget, QLabel* infolabel, QObject *parent = 0);
        DisassemblerListing* listing() const;

    protected:
        virtual void run();

    private:
        bool _elaborateinstructions;
        bool _analyzelisting;
        DisassemblerListing* _listing;
        QHexEditData* _hexeditdata;
        DisassemblerDefinition* _disassemblerdefinition;
        LogWidget* _logwidget;
        QLabel* _infolabel;
};

#endif // DISASSEMBLERWORKER_H
