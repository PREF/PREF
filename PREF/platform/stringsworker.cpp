#include "stringsworker.h"
#include "../platform/loadeddata.h"

StringsWorker::StringsWorker(ByteElaborator::StringList &stringlist, QHexEditData *hexeditdata, QObject *parent) : BasicWorker(hexeditdata, parent), _stringlist(stringlist)
{

}

void StringsWorker::run()
{
    if(!this->_hexeditdata)
        return;

    this->_cancontinue = true;

    emit searchEnable(false);

    LoadedData loadeddata(this->_hexeditdata);
    ByteElaborator::scanStrings(this->_stringlist, &loadeddata, 0, this->_hexeditdata->length(), &this->_cancontinue);

    emit searchEnable(true);
}
