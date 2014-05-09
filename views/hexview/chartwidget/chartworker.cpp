#include "chartworker.h"

ChartWorker::ChartWorker(QObject *parent): QThread(parent), _hexeditdata(nullptr)
{

}

const QList<qint64> &ChartWorker::occurrences() const
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

    qint64 maxocc = 0;
    QHexEditDataReader reader(this->_hexeditdata);

    this->_occurrences.clear();

    for(int c = 0x00; c <= 0xFF; c++)
        this->_occurrences.append(0);

    for(qint64 i = 0; i < this->_hexeditdata->length(); i++)
    {
        uchar b = reader.at(i);
        this->_occurrences[b] += 1;

        if(this->_occurrences[b] > maxocc)
            maxocc = this->_occurrences[b];
    }
}
