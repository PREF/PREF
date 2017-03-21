#include "csvexporterdialog.h"
#include "ui_csvexporterdialog.h"

CSVExporterDialog::CSVExporterDialog(const QString &file, QAbstractItemModel *model, QWidget *parent): QDialog(parent), ui(new Ui::CSVExporterDialog)
{
    ui->setupUi(this);
    ui->progressBar->setMaximum(model->rowCount());

    this->_worker = new CSVExporterWorker(file, model, this);
    connect(ui->pbCancel, SIGNAL(clicked()), this->_worker, SLOT(cancel()));
    connect(this->_worker, SIGNAL(finished()), this, SLOT(close()));
    connect(this->_worker, SIGNAL(rowProcessed(int)), ui->progressBar, SLOT(setValue(int)), Qt::QueuedConnection);

    this->_worker->start();
}

CSVExporterDialog::~CSVExporterDialog()
{
    delete ui;
}
