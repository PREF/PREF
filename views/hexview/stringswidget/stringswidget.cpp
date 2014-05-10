#include "stringswidget.h"
#include "ui_stringswidget.h"

StringsWidget::StringsWidget(QWidget *parent) : QWidget(parent), ui(new Ui::StringsWidget), _hexeditdata(nullptr), _stringsmodel(nullptr)
{
    ui->setupUi(this);
    this->createStringListActions();
}

void StringsWidget::scan(QHexEditData *hexeditdata)
{
    if(!hexeditdata || !hexeditdata->length())
        return;

    this->_hexeditdata = hexeditdata;
    this->_stringsmodel = new StringsModel(hexeditdata);
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
    delete ui;
}

void StringsWidget::createStringListActions()
{
    this->_slmenu = new QMenu(this);

    this->_actgoto = this->_slmenu->addAction("Goto");
    this->_slmenu->addSeparator();
    this->_actcopy = this->_slmenu->addAction("Copy");
    this->_actcopyoffset = this->_slmenu->addAction("Copy Offset");
    this->_actcopystring = this->_slmenu->addAction("Copy String");

    connect(this->_actgoto, SIGNAL(triggered()), this, SLOT(onGotoTriggered()));
    connect(this->_actcopy, SIGNAL(triggered()), this, SLOT(onCopyTriggered()));
    connect(this->_actcopystring, SIGNAL(triggered()), this, SLOT(onCopyStringTriggered()));
    connect(this->_actcopyoffset, SIGNAL(triggered()), this, SLOT(onCopyOffsetTriggered()));
}

void StringsWidget::onStringsWorkerFinished()
{
    this->_stringsmodel->setData(this->_worker.offsetList(), this->_worker.strings());
}

void StringsWidget::onGotoTriggered()
{
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        StringsModel::StringRange range = this->_stringsmodel->range(mi.row());
        emit gotoTriggered(range.first, range.second);
    }
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

    if(selmodel && selmodel->selectedRows().length() == 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        qApp->clipboard()->setText(QString("%1").arg(this->_stringsmodel->offset(mi.row()), 8, 16, QLatin1Char('0')).toUpper());
    }
}

void StringsWidget::onCopyStringTriggered()
{
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        qApp->clipboard()->setText(this->_stringsmodel->string(mi.row()));
    }
}

void StringsWidget::on_stringList_customContextMenuRequested(const QPoint &pos)
{
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
