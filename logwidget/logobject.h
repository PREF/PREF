#ifndef LOGOBJECT_H
#define LOGOBJECT_H

#include <QObject>
#include "logger.h"

class LogObject : public QObject
{
    Q_OBJECT

    public:
        explicit LogObject(QObject *parent = 0);
        Logger* logger() const;
        virtual void setLogger(Logger* logger);

    public slots:
        void log(const QString& msg);
        void logline(const QString& msg);
        void notice(const QString& msg);
        void warning(const QString& msg);
        void error(const QString& msg);

    protected:
        Logger* _logger;
};

#endif // LOGOBJECT_H
