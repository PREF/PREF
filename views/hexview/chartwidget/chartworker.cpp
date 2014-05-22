#include "chartworker.h"

ChartWorker::ChartWorker(QObject *parent): Worker(parent), _hexeditdata(nullptr)
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
    Worker::run();

    if(!this->_hexeditdata)
        return;

    this->_occurrences = generateOccList(this->_hexeditdata, 0, this->_hexeditdata->length(), &this->_cancontinue);
}
