#include "disassemblerview.h"
#include "ui_disassemblerview.h"

DisassemblerView::DisassemblerView(ProcessorLoader *loader, QHexEditData *hexeditdata, const QString &viewname, QLabel *labelinfo, QWidget *parent): AbstractView(hexeditdata, viewname, labelinfo, parent), ui(new Ui::DisassemblerView), _worker(nullptr), _listing(nullptr), _stringsymbols(nullptr), _loader(loader)
{
    ui->setupUi(this);

    ui->hSplitter->setStretchFactor(0, 1);
    ui->vSplitter->setStretchFactor(0, 1);

    ui->functionList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->functionList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tvVariables->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvVariables->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tvStrings->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvStrings->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    this->_toolbar = new QToolBar();
    this->_toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->_toolbar->setEnabled(false);

    ui->verticalLayout->insertWidget(0, this->_toolbar);

    this->_actback = this->_toolbar->addAction(QIcon(":/action_icons/res/back.png"), "Back");
    this->_actforward = this->_toolbar->addAction(QIcon(":/action_icons/res/forward.png"), "Forward");
    this->_actgoto = this->_toolbar->addAction(QIcon(":/action_icons/res/goto.png"), "Goto");
    this->_toolbar->addSeparator();
    this->_actentrypoints = this->_toolbar->addAction(QIcon(":/action_icons/res/entry.png"), "Entry Points");
    this->_actsegments = this->_toolbar->addAction(QIcon(":/action_icons/res/segments.png"), "Segments");

    this->_actback->setEnabled(false);
    this->_actforward->setEnabled(false);

    this->_actback->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Left));
    this->_actforward->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Right));
    this->_actgoto->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    this->_actentrypoints->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));
    this->_actsegments->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

    this->createListingMenu();
    this->createFunctionsMenu();
    this->createVariablesMenu();
    this->createStringsMenu();

    ui->disassemblerWidget->setFocus();

    connect(this->_actback, SIGNAL(triggered()), ui->disassemblerWidget, SLOT(back()));
    connect(this->_actforward, SIGNAL(triggered()), ui->disassemblerWidget, SLOT(forward()));
    connect(this->_actgoto, SIGNAL(triggered()), ui->gotoWidget, SLOT(show()));
    connect(this->_actentrypoints, SIGNAL(triggered()), this, SLOT(showEntryPoints()));
    connect(this->_actsegments, SIGNAL(triggered()), this, SLOT(showSegments()));
    connect(ui->gotoWidget, SIGNAL(addressRequested(PrefSDK::DataValue)), this, SLOT(gotoAddress(PrefSDK::DataValue)));
    connect(ui->disassemblerWidget, SIGNAL(backAvailable(bool)), this->_actback, SLOT(setEnabled(bool)));
    connect(ui->disassemblerWidget, SIGNAL(forwardAvailable(bool)), this->_actforward, SLOT(setEnabled(bool)));
    connect(ui->disassemblerWidget, SIGNAL(crossReferenceRequested(Block*)), this, SLOT(showCrossReference(Block*)));
    connect(ui->disassemblerWidget, SIGNAL(jumpToRequested()), ui->gotoWidget, SLOT(show()));
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

void DisassemblerView::createListingMenu()
{
    this->_listingmenu = new QMenu();

    this->_actcrossreferences = this->_listingmenu->addAction(QIcon(":/misc_icons/res/crossreference.png"), "Cross Refernces");
    this->_acthexdump = this->_listingmenu->addAction(QIcon(":/misc_icons/res/hex.png"), "Hex Dump");
    this->_listingmenu->addSeparator();

    QMenu* copymenu = this->_listingmenu->addMenu(QIcon(":/action_icons/res/copy.png"), "Copy");
    this->_actcopyaddress = copymenu->addAction("Address");
    this->_actcopylisting = copymenu->addAction("Listing");

    connect(this->_actcrossreferences, SIGNAL(triggered()), this, SLOT(onListingMenuCrossReferencesTriggered()));
    connect(this->_acthexdump, SIGNAL(triggered()), this, SLOT(onListingMenuHexDumpTriggered()));
    connect(this->_actcopyaddress, SIGNAL(triggered()), this, SLOT(copyAddress()));
    connect(this->_actcopylisting, SIGNAL(triggered()), this, SLOT(copyListing()));
}

void DisassemblerView::createFunctionsMenu()
{
    this->_functionsmenu = new QMenu(this);
    QAction* actjump = this->_functionsmenu->addAction(QIcon(":/action_icons/res/goto.png"), "Jump To Address");
    QAction* actxrefs = this->_functionsmenu->addAction(QIcon(":/misc_icons/res/crossreference.png"), "Cross References");

    connect(actjump, SIGNAL(triggered()), this, SLOT(gotoFunction()));
    connect(actxrefs, SIGNAL(triggered()), this, SLOT(onFunctionsMenuXRefsTriggered()));
}

void DisassemblerView::createVariablesMenu()
{
    this->_variablesmenu = new QMenu(this);
    QAction* copyvariable = this->_variablesmenu->addAction(QIcon(":/action_icons/res/copy.png"), "Copy");
    QMenu* copymenu = this->_variablesmenu->addMenu(QIcon(":/action_icons/res/copy.png"), "Copy Column...");
    QAction* copyaddress = copymenu->addAction("Address");
    QAction* copyname = copymenu->addAction("Name");
    this->_variablesmenu->addSeparator();
    QAction* exportcsv = this->_variablesmenu->addAction(QIcon(":/action_icons/res/export.png"), "Export as CSV");

    connect(copyvariable, SIGNAL(triggered()), this, SLOT(copyVariable()));
    connect(copyaddress, SIGNAL(triggered()), this, SLOT(copyVariableAddress()));
    connect(copyname, SIGNAL(triggered()), this, SLOT(copyVariableName()));
    connect(exportcsv, SIGNAL(triggered()), this, SLOT(exportVariables()));
}

void DisassemblerView::createStringsMenu()
{
    this->_stringsmenu = new QMenu(this);
    QAction* copystring = this->_stringsmenu->addAction(QIcon(":/action_icons/res/copy.png"), "Copy");
    QMenu* copymenu = this->_stringsmenu->addMenu(QIcon(":/action_icons/res/copy.png"), "Copy Column...");
    QAction* copyaddress = copymenu->addAction("Address");
    QAction* copystringval = copymenu->addAction("String");
    this->_stringsmenu->addSeparator();
    QAction* exportcsv = this->_stringsmenu->addAction(QIcon(":/action_icons/res/export.png"), "Export as CSV");

    connect(copystring, SIGNAL(triggered()), this, SLOT(copyString()));
    connect(copyaddress, SIGNAL(triggered()), this, SLOT(copyStringAddress()));
    connect(copystringval, SIGNAL(triggered()), this, SLOT(copyStringValue()));
    connect(exportcsv, SIGNAL(triggered()), this, SLOT(exportStrings()));
}

void DisassemblerView::showCrossReference(Block *b)
{
    ReferenceTable* referencetable = this->_listing->referenceTable();

    if(!referencetable->isReferenced(b))
        return;

    ReferenceSet* refset = nullptr;

    if(b->blockType() == Block::ReferenceBlock)
        refset = qobject_cast<ReferenceSet*>(b);
    else
        refset = referencetable->references(b);

    QList<Reference*> references = refset->referenceList();
    CrossReferenceDialog crd(refset, references, this->_listing);
    int res = crd.exec();

    if(res == CrossReferenceDialog::Accepted && crd.selectedBlock())
        ui->disassemblerWidget->jumpTo(crd.selectedBlock());
}

void DisassemblerView::showCrossReference(const DataValue &address)
{
    ReferenceTable* referencetable = this->_listing->referenceTable();

    if(!referencetable->isReferenced(address))
        return;

    CrossReferenceDialog crd(address, this->_listing);
    int res = crd.exec();

    if(res == CrossReferenceDialog::Accepted && crd.selectedBlock())
        ui->disassemblerWidget->jumpTo(crd.selectedBlock());
}

void DisassemblerView::disassemble(bool elaborateinstructions, bool analyzelisting)
{
    if(!this->_hexeditdata)
        return;

    this->_worker = new DisassemblerWorker(this->_hexeditdata, this->_loader, elaborateinstructions, analyzelisting, ui->logWidget, this->_lblinfo, this);
    connect(this->_worker, SIGNAL(finished()), this, SLOT(displayDisassembly()));
    this->_worker->start();
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
    QItemSelectionModel* model = ui->functionList->selectionModel();
    QModelIndex index = model->currentIndex();

    if(index.isValid())
    {
        CrossReferenceDialog crd(reinterpret_cast<Function*>(index.internalPointer()), this->_listing);
        int res = crd.exec();

        if(res == CrossReferenceDialog::Accepted && crd.selectedBlock())
            ui->disassemblerWidget->jumpTo(crd.selectedBlock());
    }
}

void DisassemblerView::onListingMenuCrossReferencesTriggered()
{
    this->showCrossReference(ui->disassemblerWidget->selectedBlock());
}

void DisassemblerView::onListingMenuHexDumpTriggered()
{    
    Block* b = ui->disassemblerWidget->selectedBlock();

    switch(b->blockType())
    {
        case Block::SegmentBlock:
        {
            Segment* s = qobject_cast<Segment*>(b);
            ui->dataView->selectRange(s->baseOffset(), s->sizeValue());
            break;
        }

        case Block::FunctionBlock:
        {
            Segment* s = this->_listing->findSegment(b);
            Function* f = qobject_cast<Function*>(b);
            ui->dataView->selectRange((f->startAddress() - s->startAddress()) + s->baseOffset(), f->sizeValue());
            break;
        }

        case Block::InstructionBlock:
        {
            Instruction* i = qobject_cast<Instruction*>(b);
            ui->dataView->selectRange(i->offsetValue(), i->sizeValue());
            break;
        }

        default:
            return;
    }

    ui->tabWidget->setCurrentIndex(1);
}

void DisassemblerView::displayDisassembly()
{
    this->_listing = this->_worker->listing();
    this->_listing->setParent(this);

    this->_toolbar->setEnabled(true);
    ui->gotoWidget->setListing(this->_listing);
    ui->disassemblerWidget->setListing(this->_listing);

    this->_functionmodel = new FunctionModel(this->_listing, ui->functionList);
    this->_stringsymbols = new StringSymbolModel(this->_listing, this->_hexeditdata, ui->tvStrings);
    this->_variablesmodel = new VariablesModel(this->_listing, this);

    ui->functionList->setModel(this->_functionmodel);
    ui->tvStrings->setModel(this->_stringsymbols);
    ui->tvVariables->setModel(this->_variablesmodel);
    ui->dataView->setModel(this->_variablesmodel);
}

void DisassemblerView::showEntryPoints()
{
    EntryPointsDialog epd(this->_listing, this);
    int res = epd.exec();

    if(res == EntryPointsDialog::Accepted && epd.selectedFunction())
        ui->disassemblerWidget->jumpTo(epd.selectedFunction());
}

void DisassemblerView::showSegments()
{
    SegmentsDialog sd(this->_listing, this);
    int res = sd.exec();

    if(res == SegmentsDialog::Accepted && sd.selectedSegment())
        ui->disassemblerWidget->jumpTo(sd.selectedSegment());
}

void DisassemblerView::gotoFunction()
{
    QItemSelectionModel* selectionmodel = ui->functionList->selectionModel();

    if(selectionmodel->hasSelection())
        ui->disassemblerWidget->jumpTo(reinterpret_cast<Function*>(selectionmodel->selectedIndexes()[0].internalPointer()));
}

void DisassemblerView::copyAddress()
{
    if(!ui->disassemblerWidget->selectedBlock())
        return;

    QClipboard* clipboard = qApp->clipboard();
        clipboard->setText(ui->disassemblerWidget->selectedBlock()->startAddress().toString(16));
}

void DisassemblerView::copyListing()
{
    if(!ui->disassemblerWidget->selectedBlock())
        return;

    QClipboard* clipboard = qApp->clipboard();
    Block* b = ui->disassemblerWidget->selectedBlock();

    if(b->blockType() == Block::InstructionBlock)
        clipboard->setText(this->_listing->formatInstruction(qobject_cast<Instruction*>(b)));
    else if(b->blockType() == Block::FunctionBlock)
    {
        const SymbolTable* symboltable = this->_listing->symbolTable();
        clipboard->setText(QString("function %1").arg(symboltable->name(b->startAddress())));
    }
    else if(b->blockType() == Block::SegmentBlock)
        clipboard->setText(QString("segment %1").arg(this->_listing->findSegment(b)->name()));
    else if(b->blockType() == Block::ReferenceBlock)
        clipboard->setText(QString("j_%1").arg(b->startAddress().toString(16)));
}

void DisassemblerView::copyVariable()
{
    QAbstractItemModel* model = ui->tvVariables->model();
    QItemSelectionModel* selectionmodel = ui->tvVariables->selectionModel();
    QModelIndexList addresslist = selectionmodel->selectedRows(0);
    QModelIndexList variablelist = selectionmodel->selectedRows(1);

    if(addresslist.isEmpty() || variablelist.isEmpty())
        return;

    QClipboard* clipboard = qApp->clipboard();
    clipboard->setText(QString("%1 %2").arg(model->data(addresslist[0]).toString(), model->data(variablelist[0]).toString()));
}

void DisassemblerView::copyVariableAddress()
{
    QAbstractItemModel* model = ui->tvVariables->model();
    QItemSelectionModel* selectionmodel = ui->tvVariables->selectionModel();
    QModelIndexList indexlist = selectionmodel->selectedRows(0);

    if(indexlist.isEmpty())
        return;

    QClipboard* clipboard = qApp->clipboard();
    clipboard->setText(model->data(indexlist[0]).toString());
}

void DisassemblerView::copyVariableName()
{
    QAbstractItemModel* model = ui->tvVariables->model();
    QItemSelectionModel* selectionmodel = ui->tvVariables->selectionModel();
    QModelIndexList indexlist = selectionmodel->selectedRows(1);

    if(indexlist.isEmpty())
        return;

    QClipboard* clipboard = qApp->clipboard();
    clipboard->setText(model->data(indexlist[0]).toString());
}

void DisassemblerView::copyString()
{
    QAbstractItemModel* model = ui->tvStrings->model();
    QItemSelectionModel* selectionmodel = ui->tvStrings->selectionModel();
    QModelIndexList addresslist = selectionmodel->selectedRows(0);
    QModelIndexList stringlist = selectionmodel->selectedRows(1);

    if(addresslist.isEmpty() || stringlist.isEmpty())
        return;

    QClipboard* clipboard = qApp->clipboard();
    clipboard->setText(QString("%1 %2").arg(model->data(addresslist[0]).toString(), model->data(stringlist[0]).toString()));
}

void DisassemblerView::copyStringAddress()
{
    QAbstractItemModel* model = ui->tvStrings->model();
    QItemSelectionModel* selectionmodel = ui->tvStrings->selectionModel();
    QModelIndexList indexlist = selectionmodel->selectedRows(0);

    if(indexlist.isEmpty())
        return;

    QClipboard* clipboard = qApp->clipboard();
    clipboard->setText(model->data(indexlist[0]).toString());
}

void DisassemblerView::copyStringValue()
{
    QAbstractItemModel* model = ui->tvStrings->model();
    QItemSelectionModel* selectionmodel = ui->tvStrings->selectionModel();
    QModelIndexList indexlist = selectionmodel->selectedRows(1);

    if(indexlist.isEmpty())
        return;

    QClipboard* clipboard = qApp->clipboard();
    clipboard->setText(model->data(indexlist[0]).toString());
}

void DisassemblerView::exportVariables()
{
    QString s = QFileDialog::getSaveFileName(this, "Export Variables...");

    if(s.isEmpty())
        return;

    CSVExporter csvexporter;
    csvexporter.dump(s, ui->tvVariables->model());
}

void DisassemblerView::exportStrings()
{
    QString s = QFileDialog::getSaveFileName(this, "Export Strings...");

    if(s.isEmpty())
        return;

    CSVExporter csvexporter;
    csvexporter.dump(s, ui->tvStrings->model());
}

void DisassemblerView::gotoAddress(const DataValue &address)
{
    Block* b = this->_listing->findBlock(address);

    if(b)
        ui->disassemblerWidget->jumpTo(b);
}

void DisassemblerView::on_functionList_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    ui->disassemblerWidget->jumpTo(reinterpret_cast<Function*>(index.internalPointer()));
}

void DisassemblerView::on_tvStrings_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    this->showCrossReference(this->_stringsymbols->string(index.row()));
}

void DisassemblerView::on_tvVariables_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    this->showCrossReference(this->_variablesmodel->variable(index.row()));
}

void DisassemblerView::on_tabOverview_currentChanged(int index)
{
    if(this->_worker->isRunning())
        return;

    switch(index)
    {
        case 0:
        {
            for(int i = 0; i < this->_variablesmodel->columnCount() - 1; i++)
                ui->tvVariables->resizeColumnToContents(i);

            ui->tvVariables->resizeRowsToContents();
            break;
        }

        case 1:
        {
            for(int i = 0; i < this->_stringsymbols->columnCount() - 1; i++)
                ui->tvStrings->resizeColumnToContents(i);

            ui->tvStrings->resizeRowsToContents();
            break;
        }

        default:
            break;
    }
}

void DisassemblerView::on_tvStrings_customContextMenuRequested(const QPoint &pos)
{
    if(this->_worker->isRunning())
        return;

    this->_stringsmenu->exec(ui->tvStrings->mapToGlobal(pos));
}

void DisassemblerView::on_tvVariables_customContextMenuRequested(const QPoint &pos)
{
    if(this->_worker->isRunning())
        return;

    this->_variablesmenu->exec(ui->tvVariables->mapToGlobal(pos));
}
