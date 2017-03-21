#ifndef LOGGER_H
#define LOGGER_H

#include "logwidget.h"

class Logger: public QObject
{
    Q_OBJECT

    public:
        explicit Logger(LogWidget *logwidget, QObject *parent = 0);

    public slots:
        void log(const QString& msg);
        void logline(const QString& msg);
        void notice(const QString& msg);
        void warning(const QString& msg);
        void error(const QString& msg);

    private:
        LogWidget* _logwidget;
};

#endif // LOGGER_H
