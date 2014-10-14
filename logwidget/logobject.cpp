#include "logobject.h"

LogObject::LogObject(QObject *parent): QObject(parent), _logger(nullptr)
{
}

Logger *LogObject::logger() const
{
    return this->_logger;
}

void LogObject::setLogger(Logger *logger)
{
    this->_logger = logger;
}

void LogObject::log(const QString &msg)
{
    if(this->_logger)
        this->_logger->log(msg);
}

void LogObject::logline(const QString &msg)
{
    if(this->_logger)
        this->_logger->logline(msg);
}

void LogObject::notice(const QString &msg)
{
    if(this->_logger)
        this->_logger->notice(msg);
}

void LogObject::warning(const QString &msg)
{
    if(this->_logger)
        this->_logger->warning(msg);
}

void LogObject::error(const QString &msg)
{
    if(this->_logger)
        this->_logger->error(msg);
}
