#include "stringsworker.h"

const qint64 StringsWorker::MIN_LENGTH = 4;
const qint64 StringsWorker::MAX_LENGTH = 256;

StringsWorker::StringsWorker(QObject *parent): Worker(parent), _hexeditdata(nullptr)
{
}

void StringsWorker::setData(QHexEditData *hexeditdata)
{
    this->_hexeditdata = hexeditdata;
}

const StringsWorker::OffsetList &StringsWorker::offsetList() const
{
    return this->_offsetlist;
}

const StringsWorker::StringMap &StringsWorker::strings() const
{
    return this->_strings;
}

void StringsWorker::run()
{
    if(!this->_hexeditdata)
        return;

    emit searchState(false); /* Disable Filter */

    this->_offsetlist.clear();
    this->_strings.clear();

    qint64 len = 0;
    QHexEditDataReader reader(this->_hexeditdata);

    for(qint64 i = 0; this->_cancontinue && (i < this->_hexeditdata->length()); i++)
    {
        uchar ch = reader.at(i);

        if(len == StringsWorker::MAX_LENGTH)
        {
            qint64 offset = i - len;
            this->_offsetlist.append(offset);
            this->_strings[offset] = StringRange(offset, len);
            len = 0;
        }
        else if(isprint(ch))
        {
            len++;
        }
        else if(len >= StringsWorker::MIN_LENGTH)
        {
            qint64 offset = i - len;
            this->_offsetlist.append(offset);
            this->_strings[offset] = StringRange(offset, len);
            len = 0;
        }
        else
            len = 0;
    }

    qSort(this->_offsetlist.begin(), this->_offsetlist.end());
    emit searchState(true); /* Enable Filter */
}
