#include "stringsworker.h"
#include "../platform/loadeddata.h"

StringsWorker::StringsWorker(ByteElaborator::StringList &stringlist, QHexDocument *document, QObject *parent) : BasicWorker(document, parent), _stringlist(stringlist)
{

}

void StringsWorker::run()
{
    if(!this->_document)
        return;

    this->_cancontinue = true;

    emit searchEnable(false);

    LoadedData loadeddata(this->_document);
    ByteElaborator::scanStrings(this->_stringlist, &loadeddata, 0, this->_document->length(), &this->_cancontinue);

    emit searchEnable(true);
}
