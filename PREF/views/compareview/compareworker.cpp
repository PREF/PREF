#include "compareworker.h"

CompareWorker::CompareWorker(QHexEdit *lefthexedit, QHexEdit *righthexedit, QObject *parent): QThread(parent), _lefthexedit(lefthexedit), _righthexedit(righthexedit)
{
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
    QHexEditData* lefthexeditdata = this->_lefthexedit->data();
    QHexEditData* righthexeditdata = this->_righthexedit->data();
    QHexEditDataReader leftreader(lefthexeditdata);
    QHexEditDataReader rightreader(righthexeditdata);
    qint64 offset = 0, end = qMax(lefthexeditdata->length(), righthexeditdata->length());

    while(offset < end)
    {
        if((offset >= lefthexeditdata->length()) || (offset >= righthexeditdata->length()))
        {
            this->_offsetlist.append(offset);
            this->_differencemap[offset] = end;
            break;
        }

        if(leftreader.at(offset) != rightreader.at(offset))
        {
            qint64 startoffset = offset++;

            while(((offset < lefthexeditdata->length()) && (offset < righthexeditdata->length())) && (leftreader.at(offset) != rightreader.at(offset)))
                offset++;

            this->_offsetlist.append(startoffset);
            this->_differencemap[startoffset] = offset - 1;
            continue;
        }

        offset++;
    }

    qSort(this->_offsetlist.begin(), this->_offsetlist.end());
}
