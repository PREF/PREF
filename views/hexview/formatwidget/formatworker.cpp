#include "formatworker.h"

FormatWorker::FormatWorker(FormatDefinition* formatdefinition, LogWidget *logwidget, QHexEditData* hexeditdata, qint64 startoffset, QObject *parent): QThread(parent), _formattree(nullptr), _formatdefinition(formatdefinition), _hexeditdata(hexeditdata), _startoffset(startoffset)
{
    this->_logger = new Logger(logwidget);
    this->_databuffer = new QDataBuffer(this->_hexeditdata);
}

FormatTree *FormatWorker::tree() const
{
    return this->_formattree;
}

void FormatWorker::run()
{
    this->_formattree = this->_formatdefinition->build(this->_databuffer, this->_startoffset);

    if(this->_formattree && !this->_formattree->isEmpty())
        emit parsingCompleted();

    if(!this->_formattree || this->_formattree->isEmpty())
        emit parsingFailed();

    this->_logger->deleteLater();
}
