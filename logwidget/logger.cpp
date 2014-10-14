#include "logger.h"

Logger::Logger(LogWidget* logwidget, QObject *parent): QObject(parent), _logwidget(logwidget)
{

}

void Logger::log(const QString &msg)
{
    QMetaObject::invokeMethod(this->_logwidget, "write", Qt::QueuedConnection, Q_ARG(QString, msg));
}

void Logger::logline(const QString &msg)
{
    QMetaObject::invokeMethod(this->_logwidget, "writeLine", Qt::QueuedConnection, Q_ARG(QString, msg));
}

void Logger::notice(const QString &msg)
{
    QMetaObject::invokeMethod(this->_logwidget, "writeNotice", Qt::QueuedConnection, Q_ARG(QString, msg));
}

void Logger::warning(const QString &msg)
{
    QMetaObject::invokeMethod(this->_logwidget, "writeWarning", Qt::QueuedConnection, Q_ARG(QString, msg));
}

void Logger::error(const QString &msg)
{
    QMetaObject::invokeMethod(this->_logwidget, "writeError", Qt::QueuedConnection, Q_ARG(QString, msg));
}
