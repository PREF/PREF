#include "compareworker.h"

CompareWorker::CompareWorker(QObject *parent): QThread(parent), _lefthexeditdata(nullptr), _righthexeditdata(nullptr)
{

}

void CompareWorker::setData(QHexEditData *lefthexeditdata, QHexEditData *righthexeditdata)
{
    this->_lefthexeditdata = lefthexeditdata;
    this->_righthexeditdata = righthexeditdata;
}

const CompareWorker::OffsetList &CompareWorker::offsetList() const
{
    return this->_offsetlist;
}

const CompareWorker::DifferenceMap &CompareWorker::differences() const
{
    return this->_differencemap;
}

void CompareWorker::run()
{
    if(!this->_lefthexeditdata || !this->_righthexeditdata)
        return;

    this->_offsetlist.clear();
    this->_differencemap.clear();

    QHexEditDataReader leftreader(this->_lefthexeditdata);
    QHexEditDataReader rightreader(this->_righthexeditdata);
    qint64 offset = 0, end = qMax(this->_lefthexeditdata->length(), this->_righthexeditdata->length());

    while(offset < end)
    {
        if((offset >= this->_lefthexeditdata->length()) || (offset >= this->_righthexeditdata->length()))
        {
            this->_offsetlist.append(offset);
            this->_differencemap[offset] = end;
            break;
        }

        if(leftreader.at(offset) != rightreader.at(offset))
        {
            qint64 startoffset = offset;

            while(((offset < this->_lefthexeditdata->length()) && (offset < this->_righthexeditdata->length())) && (leftreader.at(offset) != rightreader.at(offset)))
                offset++;

            this->_offsetlist.append(startoffset);
            this->_differencemap[startoffset] = offset - 1;
            continue;
        }

        offset++;
    }

    qSort(this->_offsetlist.begin(), this->_offsetlist.end());
}
