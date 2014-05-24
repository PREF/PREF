#include "disassemblerview.h"
#include "ui_disassemblerview.h"

DisassemblerView::DisassemblerView(QHexEditData *hexeditdata, LoaderList::LoaderId loaderid, const QString &viewname, QLabel *labelinfo, QWidget *parent): AbstractView(viewname, labelinfo, parent), ui(new Ui::DisassemblerView), _hexeditdata(hexeditdata), _loaderid(loaderid)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);

    this->_toolbar = new QToolBar();
    this->_toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->_toolbar->setEnabled(false);
    ui->verticalLayout_2->insertWidget(0, this->_toolbar);
    this->createFunctionsMenu();

    this->_actentrypoints = this->_toolbar->addAction(QIcon(":/action_icons/res/entry.png"), "Entry Points");
    this->_actsegments = this->_toolbar->addAction(QIcon(":/action_icons/res/segments.png"), "Segments");
    this->_toolbar->addSeparator();
    this->_actgoto = this->_toolbar->addAction(QIcon(":/action_icons/res/goto.png"), "Goto");

    this->_functionrefs = new FunctionOffsetModel(ui->tvFunctions);
    this->_stringrefs = new StringOffsetModel(this->_hexeditdata, ui->tvStrings);

    ui->tvFunctions->setModel(this->_functionrefs);
    ui->tvStrings->setModel(this->_stringrefs);

    this->disassemble();
}

DisassemblerView::~DisassemblerView()
{
    delete ui;
}

bool DisassemblerView::canSave() const
{
    return false;
}

void DisassemblerView::updateStatusBar()
{
    this->updateInfoText(QString());
}

void DisassemblerView::createFunctionsMenu()
{
    this->_functionsmenu = new QMenu();
    QAction* actgoto = this->_functionsmenu->addAction("Goto Address");
    QAction* actxrefs = this->_functionsmenu->addAction("Cross References");

    connect(actgoto, SIGNAL(triggered()), this, SLOT(selectVA()));
    connect(actxrefs, SIGNAL(triggered()), this, SLOT(onFunctionsMenuXRefsTriggered()));
}

void DisassemblerView::disassemble()
{
    if(!this->_hexeditdata)
        return;

    qint64 instructionscount = 0;

    lua_State* l = SDKManager::state();
    lua_State* thread = lua_newthread(l);

    lua_getglobal(thread, "Sdk");
    lua_getfield(thread, -1, "disassembleData");
    lua_pushlightuserdata(thread, this->_hexeditdata);
    lua_pushstring(thread, this->_loaderid);
    int res = lua_resume(thread, 2);

    if(res)
        DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(thread, -1)));
    else
        instructionscount = lua_tointeger(thread, -1);

    lua_pop(thread, 2);
    lua_pop(l, 1);

    this->displayDisassembly(instructionscount);
}

void DisassemblerView::on_tvFunctions_customContextMenuRequested(const QPoint &pos)
{
    if(ui->tvFunctions->selectionModel())
        this->_functionsmenu->exec(ui->tvFunctions->mapToGlobal(pos));
}

void DisassemblerView::on_tvFunctions_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
        this->selectVA();
}

void DisassemblerView::onFunctionsMenuXRefsTriggered()
{
    QItemSelectionModel* model = ui->tvFunctions->selectionModel();
    QModelIndex index = model->currentIndex();

    if(index.isValid())
    {
        //DisassembledInstruction::Ptr func = this->_disasmlisting->function(index.row());

        //CrossReferenceDialog crd(func, this->_disasmlisting);
        //int res = crd.exec();

        //if(res == CrossReferenceDialog::Accepted)
            //ui->disassemblerWidget->gotoVA(crd.instruction().VirtualAddress);
    }
}

void DisassemblerView::displayDisassembly(qint64 instructionscount)
{
    this->_toolbar->setEnabled(true);
    ui->disassemblerWidget->setInstructionCount(instructionscount);
    ui->disassemblerWidget->setData(this->_hexeditdata);

    for(int i = 0; i < ui->tvFunctions->model()->columnCount(); i++)
        ui->tvFunctions->resizeColumnToContents(i);

    /* Disassembly Page */
    //ui->disassemblerWidget->setListing(this->_disasmhelper->listing());
    //ui->disassemblerWidget->gotoEP();

    /* Function Reference Part */
    //this->_functionrefs->setListing(this->_disasmlisting_old);

    /* String Reference Part */
    //this->_stringrefs->setListing(this->_disasmlisting_old);

    /* DataMap Page */
    //ui->dataMapView->setListing(this->_disasmlisting_old);
    //ui->dataMapView->setHexEditData(this->_hexeditdata)
}

void DisassemblerView::selectVA()
{
    QItemSelectionModel* model = ui->tvFunctions->selectionModel();

    if(!model)
        return;

    QModelIndex index = model->selectedIndexes()[0];

    if(!index.isValid())
        return;

    //DisassembledInstruction func = this->_disasmlisting->function(index.row());
    //ui->disassemblerWidget->gotoVA(func.VirtualAddress);
}
