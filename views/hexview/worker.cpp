#include "worker.h"

Worker::Worker(QObject *parent): QThread(parent), _cancontinue(true), _inerror(false)
{

}

bool Worker::inError() const
{
    return this->_inerror;
}

void Worker::abort()
{
    this->_cancontinue = false;
}

void Worker::run()
{
    this->_inerror = false; /* Reset Error State */
}

void Worker::workerError(const QString &s)
{
    this->_inerror = true;
    emit error(s);
}
