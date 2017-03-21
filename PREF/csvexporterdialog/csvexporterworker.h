#ifndef CSVEXPORTERWORKER_H
#define CSVEXPORTERWORKER_H

#include <QThread>
#include <QAbstractItemModel>
#include <QFile>

class CSVExporterWorker : public QThread
{
    Q_OBJECT

    public:
        explicit CSVExporterWorker(const QString& file, QAbstractItemModel* model, QObject *parent = 0);

    public slots:
        void cancel();

    private:
        virtual void writeHeader(QFile& f);

    protected:
        virtual void run();

    signals:
        void rowProcessed(int count);

    private:
        QString _file;
        QAbstractItemModel* _model;
        bool _cancontinue;
};

#endif // CSVEXPORTERWORKER_H
