#include "disassemblerviewpage.h"
#include "ui_disassemblerviewpage.h"

DisassemblerViewPage::DisassemblerViewPage(ByteBuffer *bytebuffer, QWidget *parent): QWidget(parent), ui(new Ui::DisassemblerViewPage)
{
    this->_formatmodel = NULL;
    this->_bytebuffer = bytebuffer;

    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);

    this->_toolbar = new QToolBar();
    this->_toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->verticalLayout_2->insertWidget(0, this->_toolbar);

    this->createFunctionsMenu();

    this->_actstop = this->_toolbar->addAction(QIcon(":/misc_icons/res/stop.png"), "Stop");
    this->_actdisassemble = this->_toolbar->addAction(QIcon(":/misc_icons/res/start.png"), "Disassemble");
    this->_toolbar->addSeparator();
    this->_actgoto = this->_toolbar->addAction(QIcon(":/action_icons/res/goto.png"), "Goto");

    this->_actstop->setEnabled(false);
    this->_actgoto->setEnabled(false);

    this->_disasmhelper = new DisassemblerHelper(this);
    this->_functionrefs = new FunctionOffsetModel(ui->tvFunctions);
    this->_stringrefs = new StringOffsetModel(this->_bytebuffer, ui->tvStrings);

    ui->tvFunctions->setModel(this->_functionrefs);
    ui->tvStrings->setModel(this->_stringrefs);

    this->_toolbar->setEnabled(false);

    connect(this->_disasmhelper, SIGNAL(finished()), this, SLOT(displayDisassembly()), Qt::QueuedConnection);
    connect(this->_disasmhelper, SIGNAL(finished()), this, SLOT(resizeColumns()), Qt::QueuedConnection);

    connect(this->_actstop, SIGNAL(triggered()), this, SLOT(onActStopTriggered()));
    connect(this->_actdisassemble, SIGNAL(triggered()), this, SLOT(onActDisassembleTriggered()));
}

void DisassemblerViewPage::setData(FormatModel *formatmodel, FormatDefinition::Ptr formatdefinition)
{
    this->_formatmodel = formatmodel;
    this->_formatdefinition = formatdefinition;

    if(formatmodel && formatdefinition)
        this->_toolbar->setEnabled(true);
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
    this->_actstop->setEnabled(true);
    this->_actgoto->setEnabled(false);
    this->_actdisassemble->setEnabled(false);

    DisassemblerLoader::Ptr loader = this->_formatdefinition->generateLoader(this->_formatmodel, this->_bytebuffer);
    ui->disassemblyView->setLoader(loader);

    QtConcurrent::run(this->_disasmhelper, &DisassemblerHelper::run, this->_bytebuffer, this->_formatdefinition, loader);
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

void DisassemblerViewPage::resizeColumns()
{
    for(int i = 0; i < ui->tvFunctions->model()->columnCount(); i++)
        ui->tvFunctions->resizeColumnToContents(i);
}

void DisassemblerViewPage::on_tvFunctions_customContextMenuRequested(const QPoint &pos)
{
    if(ui->tvFunctions->selectionModel())
        this->_functionsmenu->exec(ui->tvFunctions->mapToGlobal(pos));
}

void DisassemblerViewPage::displayDisassembly()
{
    this->_actstop->setEnabled(false);
    this->_actgoto->setEnabled(true);
    this->_actdisassemble->setEnabled(true);

    /* Disassembly Page */
    ui->disassemblyView->setListing(this->_disasmhelper->listing());
    //ui->disassemblyView->gotoEP();

    /* Function Reference Part */
    //this->_functionrefs->setListing(this->_disasmlisting_old);

    /* String Reference Part */
    //this->_stringrefs->setListing(this->_disasmlisting_old);

    /* DataMap Page */
    //ui->dataMapView->setListing(this->_disasmlisting_old);
    //ui->dataMapView->setHexEditData(this->_hexeditdata);
}

void DisassemblerViewPage::on_tvFunctions_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
        this->selectVA();
}
