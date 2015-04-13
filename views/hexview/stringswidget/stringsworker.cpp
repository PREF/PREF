#include "stringsworker.h"

StringsWorker::StringsWorker(QObject *parent): Worker(parent), _hexeditdata(nullptr)
{
}

void StringsWorker::setData(QHexEditData *hexeditdata)
{
    this->_hexeditdata = hexeditdata;
}

const ByteElaborator::StringList &StringsWorker::stringList() const
{
    return this->_stringlist;
}

void StringsWorker::run()
{
    if(!this->_hexeditdata)
        return;

    emit searchState(false); /* Disable Filter */

    QDataBuffer databuffer(this->_hexeditdata);
    ByteElaborator::scanStrings(this->_stringlist, &databuffer, 0, this->_hexeditdata->length(), &this->_cancontinue);
    emit searchState(true); /* Enable Filter */
}
