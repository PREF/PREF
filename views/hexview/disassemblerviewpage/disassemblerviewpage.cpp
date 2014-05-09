#include "disassemblerviewpage.h"
#include "ui_disassemblerviewpage.h"

DisassemblerViewPage::DisassemblerViewPage(QHexEditData *hexeditdata, FormatTree *formattree, QWidget *parent): QWidget(parent), ui(new Ui::DisassemblerViewPage), _formattree(formattree), _hexeditdata(hexeditdata)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);

    this->_toolbar = new ElaborateToolBar();
    ui->verticalLayout_2->insertWidget(0, this->_toolbar);
    this->createFunctionsMenu();

    this->_toolbar->addSeparator();
    this->_actgoto = this->_toolbar->addAction(QIcon(":/action_icons/res/goto.png"), "Goto");
    this->_actgoto->setEnabled(false);

    this->_disasmhelper = new DisassemblerHelper(this);
    this->_functionrefs = new FunctionOffsetModel(ui->tvFunctions);
    this->_stringrefs = new StringOffsetModel(this->_hexeditdata, ui->tvStrings);

    ui->tvFunctions->setModel(this->_functionrefs);
    ui->tvStrings->setModel(this->_stringrefs);

    connect(this->_disasmhelper, SIGNAL(error(QString)), DebugDialog::instance(), SLOT(out(QString)), Qt::QueuedConnection);
    connect(this->_disasmhelper, SIGNAL(finished(quint64)), this, SLOT(displayDisassembly(quint64)), Qt::QueuedConnection);

    connect(this->_toolbar, SIGNAL(stopTriggered()), this, SLOT(onActStopTriggered()));
    connect(this->_toolbar, SIGNAL(startTriggered()), this, SLOT(onActDisassembleTriggered()));
}

DisassemblerViewPage::~DisassemblerViewPage()
{
    delete ui;
}

void DisassemblerViewPage::createFunctionsMenu()
{
    this->_functionsmenu = new QMenu();
    QAction* actgoto = this->_functionsmenu->addAction("Goto Address");
    QAction* actxrefs = this->_functionsmenu->addAction("Cross References");

    connect(actgoto, SIGNAL(triggered()), this, SLOT(selectVA()));
    connect(actxrefs, SIGNAL(triggered()), this, SLOT(onFunctionsMenuXRefsTriggered()));
}

void DisassemblerViewPage::onActStopTriggered()
{
    this->_disasmhelper->stop();
}

void DisassemblerViewPage::onActDisassembleTriggered()
{
    this->_actgoto->setEnabled(false);

    //QtConcurrent::run(this->_disasmhelper, &DisassemblerHelper::run, this->_hexeditdata);
    this->_disasmhelper->run(this->_hexeditdata);
}

void DisassemblerViewPage::onFunctionsMenuXRefsTriggered()
{
    QItemSelectionModel* model = ui->tvFunctions->selectionModel();
    QModelIndex index = model->currentIndex();

    if(index.isValid())
    {
        //DisassembledInstruction::Ptr func = this->_disasmlisting->function(index.row());

        //CrossReferenceDialog crd(func, this->_disasmlisting);
        //int res = crd.exec();

        //if(res == CrossReferenceDialog::Accepted)
            //ui->disassemblyView->gotoVA(crd.instruction().VirtualAddress);
    }
}

void DisassemblerViewPage::selectVA()
{
    QItemSelectionModel* model = ui->tvFunctions->selectionModel();

    if(!model)
        return;

    QModelIndex index = model->selectedIndexes()[0];

    if(!index.isValid())
        return;

    //DisassembledInstruction func = this->_disasmlisting->function(index.row());
    //ui->disassemblyView->gotoVA(func.VirtualAddress);
}

void DisassemblerViewPage::on_tvFunctions_customContextMenuRequested(const QPoint &pos)
{
    if(ui->tvFunctions->selectionModel())
        this->_functionsmenu->exec(ui->tvFunctions->mapToGlobal(pos));
}

void DisassemblerViewPage::displayDisassembly(quint64 instructionscount)
{
    this->_actgoto->setEnabled(true);
    ui->disassemblyView->setInstructionCount(instructionscount);
    ui->disassemblyView->setData(this->_hexeditdata);

    for(int i = 0; i < ui->tvFunctions->model()->columnCount(); i++)
        ui->tvFunctions->resizeColumnToContents(i);

    /* Disassembly Page */
    //ui->disassemblyView->setListing(this->_disasmhelper->listing());
    //ui->disassemblyView->gotoEP();

    /* Function Reference Part */
    //this->_functionrefs->setListing(this->_disasmlisting_old);

    /* String Reference Part */
    //this->_stringrefs->setListing(this->_disasmlisting_old);

    /* DataMap Page */
    //ui->dataMapView->setListing(this->_disasmlisting_old);
    //ui->dataMapView->setHexEditData(this->_hexeditdata);

    this->_toolbar->elaborationCompleted();
}

void DisassemblerViewPage::on_tvFunctions_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
        this->selectVA();
}
