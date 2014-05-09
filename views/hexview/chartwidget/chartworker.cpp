#include "chartworker.h"

ChartWorker::ChartWorker(QObject *parent): QThread(parent), _hexeditdata(nullptr)
{

}

const OccurrenceList &ChartWorker::occurrences() const
{
    return this->_occurrences;
}

void ChartWorker::setData(QHexEditData *hexeditdata)
{
    this->_hexeditdata = hexeditdata;
}

void ChartWorker::run()
{
    if(!this->_hexeditdata)
        return;

    this->_occurrences = generateOccList(this->_hexeditdata, 0, this->_hexeditdata->length());
}
