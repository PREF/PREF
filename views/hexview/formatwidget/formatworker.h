#ifndef FORMATWORKER_H
#define FORMATWORKER_H

#include "logwidget/logger.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/format/formatdefinition.h"
#include "prefsdk/format/formattree.h"
#include <QThread>

using namespace PrefSDK;

class FormatWorker : public QThread
{
    Q_OBJECT

    public:
        explicit FormatWorker(FormatDefinition* formatdefinition, Logger *logger, QHexEditData* hexeditdata, qint64 startoffset, QObject *parent = 0);
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
        qint64 _startoffset;
};

#endif // FORMATWORKER_H
