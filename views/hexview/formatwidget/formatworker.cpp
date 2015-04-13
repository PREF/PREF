#include "formatworker.h"

FormatWorker::FormatWorker(FormatDefinition* formatdefinition, Logger *logger, QHexEditData* hexeditdata, qint64 startoffset, QObject *parent): QThread(parent), _logger(logger), _formattree(nullptr), _formatdefinition(formatdefinition), _hexeditdata(hexeditdata), _startoffset(startoffset)
{
    /* FIXME:
    formatdefinition->moveToThread(this);
    logger->moveToThread(this);
    */
}

FormatTree *FormatWorker::tree() const
{
    return this->_formattree;
}

void FormatWorker::run()
{
    /* FIXME:
    this->_formattree = this->_formatdefinition->callParse(this->_hexeditdata, this->_logger, this->_startoffset);
    this->_formatdefinition->moveToThread(qApp->instance()->thread());

    if(this->_formattree && !this->_formattree->isEmpty())
    {
        this->_formattree->moveToThread(qApp->instance()->thread());
        emit parsingCompleted();
    }

    if(!this->_formattree || this->_formattree->isEmpty())
        emit parsingFailed();

    this->_logger->deleteLater();
    */
}
