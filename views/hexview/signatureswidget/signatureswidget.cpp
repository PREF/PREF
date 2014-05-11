#include "signatureswidget.h"
#include "ui_signatureswidget.h"

SignaturesWidget::SignaturesWidget(QWidget *parent): QWidget(parent), ui(new Ui::SignaturesWidget), _hexeditdata(nullptr)
{
    ui->setupUi(this);

    this->_signaturesmodel = new SignaturesModel();
    ui->signaturesTree->setModel(this->_signaturesmodel);

    connect(&this->_worker, SIGNAL(finished()), this, SLOT(onSignaturesWorkerFinished()));
}

void SignaturesWidget::scan(QHexEditData *hexeditdata)
{
    if(!hexeditdata || !hexeditdata->length())
        return;

    this->_hexeditdata = hexeditdata;
    this->_worker.setData(hexeditdata);
    this->_worker.start(QThread::LowPriority);
}

SignaturesWidget::~SignaturesWidget()
{
    delete ui;
}

void SignaturesWidget::onSignaturesWorkerFinished()
{
    this->_signaturesmodel->setData(this->_worker.offsetList(), this->_worker.signatures());
}

void SignaturesWidget::on_signaturesTree_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    qint64 offset = this->_signaturesmodel->signatureOffset(index.row());
    qint64 length = this->_signaturesmodel->signatureLength(index.row());
    emit gotoTriggered(offset, length);
}
