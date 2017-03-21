#include "logger.h"

Logger::Logger(LogWidget* logwidget, QObject *parent): QObject(parent), _logwidget(logwidget)
{

}

void Logger::log(const QString &msg)
{
    QMetaObject::invokeMethod(this->_logwidget, "log", Qt::QueuedConnection, Q_ARG(QString, msg));
}

void Logger::logline(const QString &msg)
{
    QMetaObject::invokeMethod(this->_logwidget, "logline", Qt::QueuedConnection, Q_ARG(QString, msg));
}

void Logger::notice(const QString &msg)
{
    QMetaObject::invokeMethod(this->_logwidget, "notice", Qt::QueuedConnection, Q_ARG(QString, msg));
}

void Logger::warning(const QString &msg)
{
    QMetaObject::invokeMethod(this->_logwidget, "warning", Qt::QueuedConnection, Q_ARG(QString, msg));
}

void Logger::error(const QString &msg)
{
    QMetaObject::invokeMethod(this->_logwidget, "error", Qt::QueuedConnection, Q_ARG(QString, msg));
}
