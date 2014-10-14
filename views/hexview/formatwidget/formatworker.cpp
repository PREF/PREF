#include "formatworker.h"

FormatWorker::FormatWorker(FormatDefinition* formatdefinition, LogWidget* logwidget, QHexEditData* hexeditdata, qint64 startoffset, QObject *parent): QThread(parent), _logwidget(logwidget), _formattree(nullptr), _formatdefinition(formatdefinition), _hexeditdata(hexeditdata), _startoffset(startoffset)
{
    formatdefinition->moveToThread(this);
}

FormatTree *FormatWorker::tree() const
{
    return this->_formattree;
}

void FormatWorker::run()
{
    Logger* logger = new Logger(this->_logwidget);
    bool validated = this->_formatdefinition->callValidate(this->_hexeditdata, logger, this->_startoffset);

    if(validated)
        this->_formattree = this->_formatdefinition->callParse(this->_hexeditdata, logger, this->_startoffset);

    this->_formatdefinition->moveToThread(qApp->instance()->thread());

    if(this->_formattree && !this->_formattree->isEmpty())
    {
        this->_formattree->moveToThread(qApp->instance()->thread());
        emit parsingCompleted();
    }

    if(!validated || !this->_formattree || this->_formattree->isEmpty())
        emit parsingFailed();
}
