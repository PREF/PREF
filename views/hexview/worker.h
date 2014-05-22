#ifndef WORKER_H
#define WORKER_H

#include <QThread>

class Worker : public QThread
{
    Q_OBJECT

    public:
        explicit Worker(QObject *parent = 0);
        bool inError() const;

    public slots:
        virtual void abort();
        virtual void run();

    protected:
        void workerError(const QString& s);

    signals:
        void error(QString errmsg);

    protected:
        bool _cancontinue;
        bool _inerror;
};

#endif // WORKER_H
