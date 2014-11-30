#include "chartworker.h"

const quint64 ChartWorker::NUM_POINTS = 2048u;
const quint64 ChartWorker::MIN_BLOCK_SIZE = 32u;

ChartWorker::ChartWorker(QObject *parent): Worker(parent), _hexeditdata(nullptr)
{

}

const OccurrenceList &ChartWorker::occurrences() const
{
    return this->_occurrences;
}

const QList<QPointF> &ChartWorker::dataEntropy() const
{
    return this->_dataentropy;
}

void ChartWorker::setData(QHexEditData *hexeditdata)
{
    this->_hexeditdata = hexeditdata;
}

quint64 ChartWorker::calculateBlockSize()
{
    if(static_cast<quint64>(this->_hexeditdata->length()) < ChartWorker::MIN_BLOCK_SIZE)
        return 0;

    quint64 blocksize = 0, numpoints = ChartWorker::NUM_POINTS, size = this->_hexeditdata->length() / numpoints;

    while(size < ChartWorker::MIN_BLOCK_SIZE)
    {
        numpoints = numpoints / 2u;
        size = this->_hexeditdata->length() / numpoints;
    }

    for(quint64 i = 0; blocksize < size; i++)
    {
        quint64 b = (1u << i);

        if(b > size)
            break;

        blocksize = b;
    }

    return blocksize;
}

void ChartWorker::run()
{
    if(!this->_hexeditdata)
        return;

    QHexEditDataReader reader(this->_hexeditdata);
    this->_occurrences = generateOccList(&reader, 0, this->_hexeditdata->length(), &this->_cancontinue);

    if(!this->_cancontinue)
        return;

    emit occurrencesListCompleted();

    quint64 blocksize = this->calculateBlockSize();
    this->_dataentropy.clear();

    if(!blocksize)
        return;

    for(quint64 i = 0; this->_cancontinue && (i < static_cast<quint64>(this->_hexeditdata->length())); i += blocksize)
    {
        OccurrenceList occlist = generateOccList(&reader, i, blocksize, &this->_cancontinue);

        if(!this->_cancontinue)
            return;

        qreal e = entropy(occlist, blocksize);
        this->_dataentropy.append(QPointF(i, e));
    }

    if(!this->_cancontinue)
        return;

    emit dataEntropyCompleted();
}
