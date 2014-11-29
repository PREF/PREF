#include "signatureswidget.h"
#include "ui_signatureswidget.h"

SignaturesWidget::SignaturesWidget(QWidget *parent): WorkerTab(parent), ui(new Ui::SignaturesWidget), _hexeditdata(nullptr), _slmenu(nullptr)
{
    ui->setupUi(this);

    this->_signaturesmodel = new SignaturesModel(ui->signaturesTree);
    ui->signaturesTree->setModel(this->_signaturesmodel);

    connect(&this->_worker, SIGNAL(started()), this, SIGNAL(workStarted()));
    connect(&this->_worker, SIGNAL(finished()), this, SLOT(onSignaturesWorkerFinished()));
    connect(&this->_worker, SIGNAL(finished()), this, SIGNAL(workFinished()));
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
    if(this->_worker.isRunning())
    {
        this->_worker.abort();
        this->_worker.wait();
    }

    delete ui;
}

void SignaturesWidget::createSignatureListMenu()
{
    this->_slmenu = new QMenu(this);

    this->_actcopy = this->_slmenu->addAction("Copy");
    this->_actcopyoffset = this->_slmenu->addAction("Copy Offset");
    this->_actcopystring = this->_slmenu->addAction("Copy String");

    connect(this->_actcopy, SIGNAL(triggered()), this, SLOT(onCopyTriggered()));
    connect(this->_actcopystring, SIGNAL(triggered()), this, SLOT(onCopyStringTriggered()));
    connect(this->_actcopyoffset, SIGNAL(triggered()), this, SLOT(onCopyOffsetTriggered()));
}

void SignaturesWidget::onSignaturesWorkerFinished()
{
    this->createSignatureListMenu();
    this->_signaturesmodel->setData(this->_worker.offsetList(), this->_worker.signatures());
}

void SignaturesWidget::onCopyTriggered()
{
    QItemSelectionModel* selmodel = ui->signaturesTree->selectionModel();

    if(selmodel && selmodel->selectedRows().length() >= 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        QString offset = QString("%1").arg(this->_signaturesmodel->signatureOffset(mi.row()), 8, 16, QLatin1Char('0')).toUpper();
        qApp->clipboard()->setText(QString("%1 \"%2\"").arg(offset, this->_signaturesmodel->signatureName(mi.row())));
    }
}

void SignaturesWidget::onCopyStringTriggered()
{
    QItemSelectionModel* selmodel = ui->signaturesTree->selectionModel();

    if(selmodel && selmodel->selectedRows().length() >= 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        qApp->clipboard()->setText(this->_signaturesmodel->signatureName(mi.row()));
    }
}

void SignaturesWidget::onCopyOffsetTriggered()
{
    QItemSelectionModel* selmodel = ui->signaturesTree->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        qApp->clipboard()->setText(QString("%1").arg(this->_signaturesmodel->signatureOffset(mi.row()), 8, 16, QLatin1Char('0')).toUpper());
    }
}

void SignaturesWidget::on_signaturesTree_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    qint64 offset = this->_signaturesmodel->signatureOffset(index.row());
    qint64 length = this->_signaturesmodel->signatureLength(index.row());
    emit gotoTriggered(offset, length);
}

void SignaturesWidget::on_signaturesTree_customContextMenuRequested(const QPoint &pos)
{
    if(!this->_slmenu)
        return;

    QItemSelectionModel* selmodel = ui->signaturesTree->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
        this->_slmenu->exec(ui->signaturesTree->mapToGlobal(pos));
}
