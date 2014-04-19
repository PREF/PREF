#include "stringfinderviewpage.h"
#include "ui_stringfinderviewpage.h"

StringFinderViewPage::StringFinderViewPage(QHexEditData* hexeditdata, QWidget *parent): QWidget(parent), ui(new Ui::StringFinderViewPage)
{
    ui->setupUi(this);

    this->_stringfinderhelper = nullptr;
    this->_hexeditdata = hexeditdata;
    this->_stringfinderhelper = new StringFinderHelper();

    this->_stringfindermodel = new StringFinderModel();
    ui->stringList->setModel(this->_stringfindermodel);

    this->createToolBar();
    this->createStringListActions();

    connect(this->_actexportcsv, SIGNAL(triggered()), this, SLOT(onExportCsvTriggered()));
    connect(this->_stringfinderhelper, SIGNAL(stringFound(quint64,QString)), this->_stringfindermodel, SLOT(addItem(quint64,QString)), Qt::QueuedConnection);
    connect(this->_stringfinderhelper, SIGNAL(progressChanged(int)), ui->progressBar, SLOT(setValue(int)), Qt::QueuedConnection);
    connect(this->_stringfinderhelper, SIGNAL(finished()), this, SLOT(onStringFinderFinished()), Qt::QueuedConnection);
    connect(this->_stringfindermodel, SIGNAL(filterChanged()), this, SLOT(onFilterChanged()));
    connect(ui->leFilter, SIGNAL(textChanged(QString)), this->_stringfindermodel, SLOT(filterString(QString)));
}

StringFinderViewPage::~StringFinderViewPage()
{
    delete ui;
}

void StringFinderViewPage::createToolBar()
{
    this->_toolbar = new ElaborateToolBar();
    this->_toolbar->addSeparator();

    this->_actexportcsv = this->_toolbar->addAction(QIcon(":/action_icons/res/export.png"), "Export CSV");
    this->_actexportcsv->setEnabled(false);

    QVBoxLayout* vl = new QVBoxLayout();
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(this->_toolbar);

    ui->tbContainer->setLayout(vl);

    connect(this->_toolbar, SIGNAL(startTriggered()), this, SLOT(onStartTriggered()));
    connect(this->_toolbar, SIGNAL(stopTriggered()), this->_stringfinderhelper, SLOT(stop()));
}

void StringFinderViewPage::checkValidState()
{
    if(ui->sbMinLength->value() >= ui->sbMaxLength->value())
        this->_toolbar->setElaborateEnabled(false);
    else
        this->_toolbar->setElaborateEnabled(true);
}

void StringFinderViewPage::createStringListActions()
{
    this->_slmenu = new QMenu(this);

    this->_actgoto = this->_slmenu->addAction("Goto");
    this->_slmenu->addSeparator();
    this->_actcopystring = this->_slmenu->addAction("Copy String");
    this->_actcopyaddress = this->_slmenu->addAction("Copy Address");

    connect(this->_actgoto, SIGNAL(triggered()), this, SLOT(onGotoTriggered()));
    connect(this->_actcopystring, SIGNAL(triggered()), this, SLOT(onCopyStringTriggered()));
    connect(this->_actcopyaddress, SIGNAL(triggered()), this, SLOT(onCopyAddressTriggered()));
}

void StringFinderViewPage::on_sbMinLength_valueChanged(int)
{
    this->checkValidState();
}

void StringFinderViewPage::on_sbMaxLength_valueChanged(int)
{
    this->checkValidState();
}

void StringFinderViewPage::onStartTriggered()
{
    dynamic_cast<StringFinderModel*>(ui->stringList->model())->clearItems();
    this->_actexportcsv->setEnabled(false);

    QtConcurrent::run(this->_stringfinderhelper, &StringFinderHelper::run, this->_hexeditdata, ui->sbMinLength->value(), ui->sbMaxLength->value());
}

void StringFinderViewPage::onGotoTriggered()
{
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        emit gotoTriggered(this->_stringfindermodel->offset(mi.row()), this->_stringfindermodel->string(mi.row()).length());
    }
}

void StringFinderViewPage::onExportCsvTriggered()
{
    QString s = QFileDialog::getSaveFileName(this, "Export Strings...", QString(), "CSV File (*.csv)");

    if(!s.isEmpty())
    {
        QFile f(s);
        f.open(QFile::WriteOnly);

        for(int i = 0; i < this->_stringfindermodel->visibleStringsCount(); i++)
        {
            QString offset = QString("%1").arg(this->_stringfindermodel->offset(i), 8, 16, QLatin1Char('0')).toUpper();
            f.write(QString("%1,%2\n").arg(offset, this->_stringfindermodel->string(i).simplified()).toLatin1());
        }

        f.close();
    }
}

void StringFinderViewPage::onCopyAddressTriggered()
{
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        qApp->clipboard()->setText(QString::number(this->_stringfindermodel->offset(mi.row()), 16).toUpper());
    }
}

void StringFinderViewPage::onCopyStringTriggered()
{
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
    {
        QModelIndex mi = selmodel->selectedRows()[0];
        qApp->clipboard()->setText(this->_stringfindermodel->string(mi.row()));
    }
}

void StringFinderViewPage::onFilterChanged()
{
    if(this->_stringfindermodel->visibleStringsCount())
        this->_actexportcsv->setEnabled(true);
    else
        this->_actexportcsv->setEnabled(false);
}

void StringFinderViewPage::onStringFinderFinished()
{
    for(int i = 0; i < ui->stringList->model()->columnCount(); i++)
        ui->stringList->resizeColumnToContents(i);

    if(this->_stringfindermodel->rowCount())
        ui->leFilter->setEnabled(true);

    if(this->_stringfindermodel->visibleStringsCount())
        this->_actexportcsv->setEnabled(true);
    else
        this->_actexportcsv->setEnabled(false);

    this->_toolbar->elaborationCompleted();
}

void StringFinderViewPage::on_stringList_customContextMenuRequested(const QPoint &pos)
{
    QItemSelectionModel* selmodel = ui->stringList->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
        this->_slmenu->exec(ui->stringList->mapToGlobal(pos));
}

void StringFinderViewPage::on_stringList_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
        emit gotoTriggered(this->_stringfindermodel->offset(index.row()), this->_stringfindermodel->string(index.row()).length());
}
