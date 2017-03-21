#include "csvexporterworker.h"

CSVExporterWorker::CSVExporterWorker(const QString &file, QAbstractItemModel *model, QObject *parent): QThread(parent), _file(file), _model(model), _cancontinue(true)
{
}

void CSVExporterWorker::cancel()
{
    this->_cancontinue = false;
}

void CSVExporterWorker::writeHeader(QFile &f)
{
    for(int i = 0; this->_cancontinue && (i < this->_model->columnCount()); i++)
    {
        if(i > 0)
            f.write(",");

        f.write(this->_model->headerData(i, Qt::Horizontal).toString().toUtf8());
    }

    f.write("\n");
}

void CSVExporterWorker::run()
{
    QFile f(this->_file);
    f.open(QFile::WriteOnly | QFile::Truncate);

    this->writeHeader(f);

    for(int i = 0; this->_cancontinue && (i < this->_model->rowCount()); i++)
    {
        for(int j = 0; this->_cancontinue && (j < this->_model->columnCount()); j++)
        {
            if(j > 0)
                f.write(",");

            f.write(this->_model->data(this->_model->index(i, j)).toString().toUtf8());
        }

        f.write("\n");
        emit rowProcessed(i + 1);
    }

    f.close();
}
