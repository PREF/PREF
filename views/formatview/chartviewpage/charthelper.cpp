#include "charthelper.h"

ChartHelper::ChartHelper(QObject *parent): QObject(parent)
{
    this->_cancontinue = true;
    this->_lastperc = this->_maxocc = 0;
    this->_hexeditdata = nullptr;
}

void ChartHelper::updateProgressBar(qint64 length, qreal newval)
{
    int perc = static_cast<int>((newval / static_cast<qreal>(length) * 100));

    if(perc != this->_lastperc)
    {
        this->_lastperc = perc;
        emit progressChanged(perc);
    }
}

void ChartHelper::run(QHexEditData *hexeditdata, qint64 start, qint64 end)
{
    if(hexeditdata->length())
    {
        this->_cancontinue = true;
        this->_maxocc = 0;

        emit progressChanged(0); /* Set progress bar to 0% */

        this->initMap();

        for(qint64 i = start; this->_cancontinue && (i < end); i++)
        {
            uchar b = hexeditdata->at(i);
            this->_occlist[b] += 1;

            if(this->_maxocc < this->_occlist[b])
                this->_maxocc = this->_occlist[b];

            this->updateProgressBar(end - start, static_cast<qreal>(i));
        }

        emit progressChanged(100); /* Set progress bar to 100% */
        emit finished();
    }
}

void ChartHelper::stop()
{
    this->_cancontinue = false;
}

QList<qint64> ChartHelper::occurrences()
{
    return this->_occlist;
}

quint64 ChartHelper::maxOccurrence()
{
    return this->_maxocc;
}

void ChartHelper::initMap()
{
    this->_occlist = QList<qint64>();

    for(int c = 0x00; c <= 0xFF; c++)
        this->_occlist.append(0);
}
