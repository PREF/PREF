#ifndef DISASSEMBLERHELPER_H
#define DISASSEMBLERHELPER_H

#include <QtCore>
#include <QtGui>
#include "prefsdk/qlua.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/format/formatdefinition.h"
#include "prefsdk/disassembler/addressqueue.h"
#include "prefsdk/disassembler/disassemblerlisting.h"

using namespace PrefSDK;

class DisassemblerHelper : public QObject
{
    Q_OBJECT

    public:
        explicit DisassemblerHelper(QObject *parent = 0);
        //void run(ByteBuffer *bytebuffer, FormatDefinitionOld::Ptr formatdefinition, DisassemblerLoader::Ptr loader);
        void stop();
        DisassemblerListing::Ptr listing();

    signals:
        void finished();

    private:
        bool _cancontinue;
        DisassemblerListing::Ptr _listing;
};

#endif // DISASSEMBLERHELPER_H
