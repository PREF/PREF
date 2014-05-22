#ifndef WORKER_H
#define WORKER_H

#include <QThread>

class Worker : public QThread
{
    Q_OBJECT

    public:
        explicit Worker(QObject *parent = 0);

    public slots:
        virtual void abort();

    signals:
        void error(QString errmsg);

    protected:
        bool _cancontinue;
};

#endif // WORKER_H
