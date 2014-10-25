#ifndef CSVEXPORTERDIALOG_H
#define CSVEXPORTERDIALOG_H

#include <QDialog>
#include "csvexporterworker.h"

namespace Ui {
class CSVExporterDialog;
}

class CSVExporterDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit CSVExporterDialog(const QString& file, QAbstractItemModel* model, QWidget *parent = 0);
        ~CSVExporterDialog();

    private:
        Ui::CSVExporterDialog *ui;
        CSVExporterWorker* _worker;
};

#endif // CSVEXPORTERDIALOG_H
