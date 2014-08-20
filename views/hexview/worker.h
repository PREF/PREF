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
        virtual void run() = 0;

    signals:
        void error(QString);

    protected:
        bool _cancontinue;
};

#endif // WORKER_H
