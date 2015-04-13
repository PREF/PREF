#include "stringswidget.h"
#include "ui_stringswidget.h"

StringsWidget::StringsWidget(QWidget *parent) : WorkerTab(parent), ui(new Ui::StringsWidget), _stringsmodel(nullptr), _slmenu(nullptr)
{
    ui->setupUi(this);

    connect(ui->leSearch, SIGNAL(returnPressed()), this, SLOT(on_tbSearchDown_clicked()));
    connect(&this->_worker, SIGNAL(started()), this, SIGNAL(workStarted()));
    connect(&this->_worker, SIGNAL(finished()), this, SIGNAL(workFinished()));
}

void StringsWidget::scan(QHexEditData *hexeditdata)
{
    if(!hexeditdata || !hexeditdata->length())
        return;

    this->_stringsmodel = new StringsModel(hexeditdata, ui->stringList);
    ui->stringList->setModel(this->_stringsmodel);

    connect(&this->_worker, SIGNAL(searchState(bool)), ui->leSearch, SLOT(setEnabled(bool)));
    connect(&this->_worker, SIGNAL(searchState(bool)), ui->tbSearchDown, SLOT(setEnabled(bool)));
    connect(&this->_worker, SIGNAL(searchState(bool)), ui->tbSearchUp, SLOT(setEnabled(bool)));
    connect(&this->_worker, SIGNAL(finished()), this, SLOT(onStringsWorkerFinished()));

    this->_worker.setData(hexeditdata);
    this->_worker.start(QThread::LowPriority);
}

StringsWidget::~StringsWidget()
{
    if(this->_worker.isRunning())
    {
        this->_worker.abort();
        this->_worker.wait();
    }

    delete ui;
}

void StringsWidget::createStringListActions()
{
    this->_slmenu = new QMenu(this);

    this->_actcopy = this->_slmenu->addAction("Copy");
    this->_actcopyoffset = this->_slmenu->addAction("Copy Offset");
    this->_actcopystring = this->_slmenu->addAction("Copy String");

    connect(this->_actcopy, SIGNAL(triggered()), this, SLOT(onCopyTriggered()));
    connect(this->_actcopystring, SIGNAL(triggered()), this, SLOT(onCopyStringTriggered()));
    connect(this->_actcopyoffset, SIGNAL(triggered()), this, SLOT(onCopyOffsetTriggered()));
}

void StringsWidget::displayNotFoundMessage()
{
    QMessageBox m;
    m.setWindowTitle("Search Error");
    m.setText(QString("String '%1' not found").arg(ui->leSearch->text()));
    m.setIcon(QMessageBox::Information);
    m.setDefaultButton(QMessageBox::Ok);
    m.exec();
}

QModelIndex StringsWidget::search(StringsModel::SearchDirection direction)
{
    QModelIndex index;
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
        index = selmodel->selectedRows()[0];

    return this->_stringsmodel->indexOf(ui->leSearch->text(), direction, index);
}

void StringsWidget::onStringsWorkerFinished()
{
    this->createStringListActions();
    this->_stringsmodel->setData(this->_worker.offsetList(), this->_worker.strings());
}

void StringsWidget::onCopyTriggered()
{
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        QString offset = QString("%1").arg(this->_stringsmodel->offset(mi.row()), 8, 16, QLatin1Char('0')).toUpper();
        qApp->clipboard()->setText(QString("%1 \"%2\"").arg(offset, this->_stringsmodel->string(mi.row())));
    }
}

void StringsWidget::onCopyOffsetTriggered()
{
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();

    if(selmodel && selmodel->selectedRows().length() >= 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        qApp->clipboard()->setText(QString("%1").arg(this->_stringsmodel->offset(mi.row()), 8, 16, QLatin1Char('0')).toUpper());
    }
}

void StringsWidget::onCopyStringTriggered()
{
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();

    if(selmodel && selmodel->selectedRows().length() >= 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        qApp->clipboard()->setText(this->_stringsmodel->string(mi.row()));
    }
}

void StringsWidget::on_stringList_customContextMenuRequested(const QPoint &pos)
{
    if(!this->_slmenu)
        return;

    QItemSelectionModel* selmodel = ui->stringList->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
        this->_slmenu->exec(ui->stringList->mapToGlobal(pos));
}

void StringsWidget::on_stringList_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    StringsModel::StringRange range = this->_stringsmodel->range(index.row());
    emit gotoTriggered(range.first, range.second);
}

void StringsWidget::on_tbSearchUp_clicked()
{
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();
    QModelIndex index = this->search(StringsModel::Up);

    if(index.isValid())
        selmodel->setCurrentIndex(index, QItemSelectionModel::Rows | QItemSelectionModel::SelectCurrent);
    else
        this->displayNotFoundMessage();
}

void StringsWidget::on_tbSearchDown_clicked()
{
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();
    QModelIndex index = this->search(StringsModel::Down);

    if(index.isValid())
        selmodel->setCurrentIndex(index, QItemSelectionModel::Rows | QItemSelectionModel::SelectCurrent);
    else
        this->displayNotFoundMessage();
}
