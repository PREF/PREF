#include "worker.h"

Worker::Worker(QObject *parent): QThread(parent), _cancontinue(true)
{

}

void Worker::abort()
{
    this->_cancontinue = false;
}
