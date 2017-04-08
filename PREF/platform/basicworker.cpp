#include "basicworker.h"

BasicWorker::BasicWorker(QHexDocument *document, QObject *parent) : QThread(parent), _document(document), _cancontinue(true)
{

}

void BasicWorker::abort()
{
    this->_cancontinue = false;
}
