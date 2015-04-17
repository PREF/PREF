#ifndef FORMATWORKER_H
#define FORMATWORKER_H

#include <QThread>
#include "format/formattree.h"
#include "format/formatdefinition.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/qdatabuffer.h"
#include "logwidget/logger.h"

using namespace PrefLib::Format;

class FormatWorker : public QThread
{
    Q_OBJECT

    public:
        explicit FormatWorker(FormatDefinition* formatdefinition, LogWidget *logwidget, QHexEditData* hexeditdata, qint64 startoffset, QObject *parent = 0);
        FormatTree* tree() const;

    protected:
        virtual void run();

    signals:
        void parsingFailed();
        void parsingCompleted();

    private:
        Logger* _logger;
        FormatTree* _formattree;
        FormatDefinition* _formatdefinition;
        QHexEditData* _hexeditdata;
        QDataBuffer* _databuffer;
        qint64 _startoffset;
};

#endif // FORMATWORKER_H
