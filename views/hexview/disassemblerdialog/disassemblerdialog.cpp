#include "disassemblerdialog.h"
#include "ui_disassemblerdialog.h"

DisassemblerDialog::DisassemblerDialog(QHexEditData *hexeditdata, FormatTree *formattree, QWidget *parent): QDialog(parent), ui(new Ui::DisassemblerDialog), _formattree(formattree), _hexeditdata(hexeditdata)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);

    this->_toolbar = new QToolBar();
    this->_toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->verticalLayout_2->insertWidget(0, this->_toolbar);
    this->createFunctionsMenu();

    this->_actgoto = this->_toolbar->addAction(QIcon(":/action_icons/res/goto.png"), "Goto");
    this->_actgoto->setEnabled(false);

    this->_functionrefs = new FunctionOffsetModel(ui->tvFunctions);
    this->_stringrefs = new StringOffsetModel(this->_hexeditdata, ui->tvStrings);

    ui->tvFunctions->setModel(this->_functionrefs);
    ui->tvStrings->setModel(this->_stringrefs);

    connect(&this->_worker, SIGNAL(error(QString)), this, SLOT(onDisassemblerError(QString)));
    connect(&this->_worker, SIGNAL(finished()), this, SLOT(displayDisassembly()));

    this->_worker.setData(hexeditdata);
    this->_worker.start(QThread::LowPriority);
}

DisassemblerDialog::~DisassemblerDialog()
{
    delete ui;
}

void DisassemblerDialog::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

void DisassemblerDialog::createFunctionsMenu()
{
    this->_functionsmenu = new QMenu();
    QAction* actgoto = this->_functionsmenu->addAction("Goto Address");
    QAction* actxrefs = this->_functionsmenu->addAction("Cross References");

    connect(actgoto, SIGNAL(triggered()), this, SLOT(selectVA()));
    connect(actxrefs, SIGNAL(triggered()), this, SLOT(onFunctionsMenuXRefsTriggered()));
}

void DisassemblerDialog::onFunctionsMenuXRefsTriggered()
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

void DisassemblerDialog::onDisassemblerError(QString msg)
{
    this->_worker.quit();
    DebugDialog::instance()->out(msg);
}

void DisassemblerDialog::selectVA()
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

void DisassemblerDialog::on_tvFunctions_customContextMenuRequested(const QPoint &pos)
{
    if(ui->tvFunctions->selectionModel())
        this->_functionsmenu->exec(ui->tvFunctions->mapToGlobal(pos));
}

void DisassemblerDialog::displayDisassembly()
{
    this->_actgoto->setEnabled(true);
    ui->disassemblyView->setInstructionCount(this->_worker.instructionCount());
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
}

void DisassemblerDialog::on_tvFunctions_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
        this->selectVA();
}
