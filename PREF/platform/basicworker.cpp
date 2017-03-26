#include "basicworker.h"

BasicWorker::BasicWorker(QHexEditData* hexeditdata, QObject *parent) : QThread(parent), _hexeditdata(hexeditdata), _cancontinue(true)
{

}

void BasicWorker::abort()
{
    this->_cancontinue = false;
}
