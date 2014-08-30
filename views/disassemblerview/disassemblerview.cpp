#include "disassemblerview.h"
#include "ui_disassemblerview.h"

DisassemblerView::DisassemblerView(LoaderList::LoaderId loaderid, QHexEditData *hexeditdata, const QString &viewname, QLabel *labelinfo, QWidget *parent): AbstractView(hexeditdata, viewname, labelinfo, parent), ui(new Ui::DisassemblerView), _listing(nullptr), _loaderid(loaderid)
{
    ui->setupUi(this);
    ui->hSplitter->setStretchFactor(0, 1);
    ui->vSplitter->setStretchFactor(0, 1);
    ui->dvSplitter->setStretchFactor(1, 1);

    ui->hexEdit->setReadOnly(true);
    ui->hexEdit->setData(hexeditdata);

    this->_toolbar = new QToolBar();
    this->_toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->_toolbar->setEnabled(false);

    ui->verticalLayout->insertWidget(0, this->_toolbar);

    this->_actentrypoints = this->_toolbar->addAction(QIcon(":/action_icons/res/entry.png"), "Entry Points");
    this->_actsegments = this->_toolbar->addAction(QIcon(":/action_icons/res/segments.png"), "Segments");
    this->_toolbar->addSeparator();
    this->_actgoto = this->_toolbar->addAction(QIcon(":/action_icons/res/goto.png"), "Goto");

    //this->_stringrefs = new StringOffsetModel(this->_hexeditdata, ui->tvStrings);

    connect(this->_actentrypoints, SIGNAL(triggered()), this, SLOT(showEntryPoints()));
    connect(this->_actsegments, SIGNAL(triggered()), this, SLOT(showSegments()));

    this->createListingMenu();
    this->createFunctionsMenu();
    this->disassemble();
}

DisassemblerView::~DisassemblerView()
{
    delete ui;
}

QHexEditData *DisassemblerView::data()
{
    return this->_hexeditdata;
}

bool DisassemblerView::canSave() const
{
    return false;
}

void DisassemblerView::updateStatusBar()
{
    this->updateInfoText(QString());
}

void DisassemblerView::log(const QString &text)
{
    ui->logWidget->write(text);
}

void DisassemblerView::logLine(const QString &text, LogWidget::LogLevel loglevel)
{
    ui->logWidget->writeLine(text, loglevel);
}

void DisassemblerView::createListingMenu()
{
    this->_listingmenu = new QMenu();

    this->_actcrossreferences = this->_listingmenu->addAction(QIcon(":/misc_icons/res/crossreference.png"), "Cross Refernces");
    this->_listingmenu->addSeparator();
    this->_actjumptoaddress = this->_listingmenu->addAction(QIcon(":/action_icons/res/goto.png"),"Jump to Address");
    this->_acthexdump = this->_listingmenu->addAction(QIcon(":/misc_icons/res/hex.png"), "Hex Dump");
    this->_listingmenu->addSeparator();

    QMenu* copymenu = this->_listingmenu->addMenu(QIcon(":/action_icons/res/copy.png"), "Copy");
    this->_actcopyaddress = copymenu->addAction("Address");
    this->_actcopylisting = copymenu->addAction("Listing");

    connect(this->_actjumptoaddress, SIGNAL(triggered()), this, SLOT(onListingMenuJumpToAddressTriggered()));
    connect(this->_acthexdump, SIGNAL(triggered()), this, SLOT(onListingMenuHexDumpTriggered()));
}

void DisassemblerView::createFunctionsMenu()
{
    this->_functionsmenu = new QMenu();
    QAction* actjump = this->_functionsmenu->addAction(QIcon(":/action_icons/res/goto.png"), "Jump To Address");
    QAction* actxrefs = this->_functionsmenu->addAction(QIcon(":/misc_icons/res/crossreference.png"), "Cross References");

    connect(actjump, SIGNAL(triggered()), this, SLOT(gotoFunction()));
    connect(actxrefs, SIGNAL(triggered()), this, SLOT(onFunctionsMenuXRefsTriggered()));
}

void DisassemblerView::disassemble()
{
    if(!this->_hexeditdata)
        return;

    this->_listing = new DisassemblerListing(this);

    lua_State* l = LuaState::instance();
    lua_State* thread = lua_newthread(l);

    lua_getglobal(thread, "Sdk");
    lua_getfield(thread, -1, "disassembleData");
    lua_pushlightuserdata(thread, this->_listing);
    lua_pushlightuserdata(thread, this->_hexeditdata);
    lua_pushstring(thread, this->_loaderid);
    int res = lua_resume(thread, 3);

    if(res)
    {
        DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(thread, -1)));
        lua_pop(thread, 2);
    }
    else
        lua_pop(thread, 1);

    lua_pop(l, 1);
    this->displayDisassembly();
}

void DisassemblerView::on_disassemblerWidget_customContextMenuRequested(const QPoint &pos)
{
    this->_listingmenu->exec(ui->disassemblerWidget->mapToGlobal(pos));
}

void DisassemblerView::on_functionList_customContextMenuRequested(const QPoint &pos)
{
    this->_functionsmenu->exec(ui->functionList->mapToGlobal(pos));
}

void DisassemblerView::onFunctionsMenuXRefsTriggered()
{
    //QItemSelectionModel* model = ui->tvFunctions->selectionModel();
    //QModelIndex index = model->currentIndex();

    //if(index.isValid())
    //{
        //DisassembledInstruction::Ptr func = this->_disasmlisting->function(index.row());

        //CrossReferenceDialog crd(func, this->_disasmlisting);
        //int res = crd.exec();

        //if(res == CrossReferenceDialog::Accepted)
            //ui->disassemblerWidget->gotoVA(crd.instruction().VirtualAddress);
    //}
}

void DisassemblerView::onListingMenuJumpToAddressTriggered()
{
    const DisassemblerWidget::ListingItem& listingitem = ui->disassemblerWidget->selectedItem();

    if(listingitem.itemType() != DisassemblerWidget::ListingItem::Reference)
        return;

    const DisassemblerListing::ReferenceSet& references = listingitem.references();

    if(references.count() == 1)
    {
        Reference* r = (*references.begin());
        ui->disassemblerWidget->gotoAddress(r->srcAddress());
    }
}

void DisassemblerView::onListingMenuHexDumpTriggered()
{
    const DisassemblerWidget::ListingItem& listingitem = ui->disassemblerWidget->selectedItem();

    if(listingitem.itemType() != DisassemblerWidget::ListingItem::Listing)
        return;

    ListingObject* listingobj = listingitem.listingObject();

    if(listingobj->objectType() != ListingTypes::Instruction)
        return;

    Instruction* instruction = qobject_cast<Instruction*>(listingobj);
    ui->hexEdit->setSelectionRange(instruction->offset(), instruction->size());
    ui->tabWidget->setCurrentIndex(1);
}

void DisassemblerView::displayDisassembly()
{
    this->_toolbar->setEnabled(true);
    this->_functionmodel = new FunctionModel(this->_listing, ui->functionList);
    ui->disassemblerWidget->setListing(this->_listing);

    ui->functionList->setModel(this->_functionmodel);
    ui->functionList->resizeRowsToContents();

    for(int i = 0; i < this->_functionmodel->columnCount(); i++)
        ui->functionList->resizeColumnToContents(i);

    /* Go to the first entry point */
    for(int i = 0; i < this->_listing->segmentsCount(); i++)
    {
        Segment* segment = this->_listing->segment(i);

        if((segment->type() != SegmentTypes::Code) || (!segment->entryPointsCount()))
            continue;

        Function* func = segment->entryPoint(0);
        ui->disassemblerWidget->selectItem(func);
    }

    /* String Reference Part */
    //this->_stringrefs->setListing(this->_disasmlisting_old);

    /* DataMap Page */
    //ui->dataMapView->setListing(this->_disasmlisting_old);
    //ui->dataMapView->setHexEditData(this->_hexeditdata)
}

void DisassemblerView::showEntryPoints()
{
    EntryPointsDialog epd(this->_listing, this);
    epd.exec();
}

void DisassemblerView::showSegments()
{
    SegmentsDialog sd(this->_listing, this);
    sd.exec();
}

void DisassemblerView::gotoFunction()
{
    QItemSelectionModel* selectionmodel = ui->functionList->selectionModel();

    if(selectionmodel->hasSelection())
        ui->disassemblerWidget->selectItem(reinterpret_cast<Function*>(selectionmodel->selectedIndexes()[0].internalPointer()));
}

void DisassemblerView::on_functionList_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    ui->disassemblerWidget->selectItem(reinterpret_cast<Function*>(index.internalPointer()));
}
