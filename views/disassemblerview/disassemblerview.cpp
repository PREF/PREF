#include "disassemblerview.h"
#include "ui_disassemblerview.h"

DisassemblerView::DisassemblerView(DisassemblerDefinition *disassemblerdefinition, bool canloaddatabase, QHexEditData *hexeditdata, const QString &loadedfile, QLabel *labelinfo, QWidget *parent): AbstractView(hexeditdata, loadedfile, labelinfo, parent), ui(new Ui::DisassemblerView), _worker(nullptr), _listing(nullptr), _stringsymbols(nullptr), _disassembler(disassemblerdefinition), _canloaddatabase(canloaddatabase)
{
    ui->setupUi(this);
    ui->hSplitter->setStretchFactor(0, 1);

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
    this->_actbookmarks = this->_toolbar->addAction(QIcon(":/action_icons/res/bookmark.png"), "Bookmarks");
    this->_actloaddatabase = this->_toolbar->addAction(QIcon(":/signature_database/res/database.png"), "Load Database");

    this->_actback->setEnabled(false);
    this->_actforward->setEnabled(false);
    this->_actbookmarks->setEnabled(false);

    this->_actback->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Left));
    this->_actforward->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Right));
    this->_actgoto->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    this->_actentrypoints->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));
    this->_actsegments->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    this->_actbookmarks->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_B));

    this->createListingMenu();
    this->createFunctionsMenu();
    this->createVariablesMenu();
    this->createStringsMenu();

    connect(this->_actback, SIGNAL(triggered()), ui->disassemblerWidget, SLOT(back()));
    connect(this->_actforward, SIGNAL(triggered()), ui->disassemblerWidget, SLOT(forward()));
    connect(this->_actgoto, SIGNAL(triggered()), ui->gotoWidget, SLOT(show()));
    connect(this->_actentrypoints, SIGNAL(triggered()), this, SLOT(showEntryPoints()));
    connect(this->_actsegments, SIGNAL(triggered()), this, SLOT(showSegments()));
    connect(this->_actbookmarks, SIGNAL(triggered()), this, SLOT(showBookmarks()));
    connect(this->_actloaddatabase, SIGNAL(triggered()), this, SLOT(loadDatabase()));
    connect(ui->gotoWidget, SIGNAL(addressRequested(PrefSDK::DataValue)), this, SLOT(gotoAddress(PrefSDK::DataValue)));
    connect(ui->bookmarkWidget, SIGNAL(saveBookmarkRequested(Block*,QString)), this, SLOT(onSaveBookmarkRequested(Block*,QString)));
    connect(ui->renameWidget, SIGNAL(renameRequested(Block*,QString)), this, SLOT(onRenameSymbolRequested(Block*,QString)));
    connect(ui->disassemblerWidget, SIGNAL(backAvailable(bool)), this->_actback, SLOT(setEnabled(bool)));
    connect(ui->disassemblerWidget, SIGNAL(forwardAvailable(bool)), this->_actforward, SLOT(setEnabled(bool)));
    connect(ui->disassemblerWidget, SIGNAL(renameRequested(Block*)), this, SLOT(renameBlock(Block*)));
    connect(ui->disassemblerWidget, SIGNAL(crossReferenceRequested(Block*)), this, SLOT(showCrossReference(Block*)));
    connect(ui->disassemblerWidget, SIGNAL(jumpToRequested()), ui->gotoWidget, SLOT(show()));
}

DisassemblerView::~DisassemblerView()
{
    delete ui;
}

void DisassemblerView::save(const QString& filename, const QString& filter)
{
    if(!QString::compare(filter, "Disassembler Database (*.db)"))
        this->_listing->save(filename);
    else if(!QString::compare(filter, "Text File (*.txt)"))
        ui->disassemblerWidget->save(filename);
}

QString DisassemblerView::saveFilter() const
{
    return "Disassembler Database (*.db);;Text File (*.txt)";
}

QHexEditData *DisassemblerView::data()
{
    return this->_hexeditdata;
}

bool DisassemblerView::canSave() const
{
    return true;
}

void DisassemblerView::updateStatusBar()
{
    this->updateInfoText(QString());
}

void DisassemblerView::createListingMenu()
{
    this->_listingmenu = new QMenu();
    connect(this->_listingmenu, SIGNAL(aboutToShow()), this, SLOT(onListingMenuAboutToShow()));

    this->_actcrossreferences = this->_listingmenu->addAction(QIcon(":/misc_icons/res/crossreference.png"), "Cross Refernces");
    this->_actrename = this->_listingmenu->addAction(QIcon(":/action_icons/res/rename.png"), "Rename");
    this->_acthexdump = this->_listingmenu->addAction(QIcon(":/misc_icons/res/hex.png"), "Hex Dump");
    this->_actaddbookmark = this->_listingmenu->addAction(QIcon(":/action_icons/res/bookmark.png"), "Add Bookmark");
    this->_actremovebookmark = this->_listingmenu->addAction(QIcon(":/action_icons/res/bookmark.png"), "Remove Bookmark");
    this->_listingmenu->addSeparator();
    this->_actcopy = this->_listingmenu->addAction("Copy");
    this->_actcopyaddress = this->_listingmenu->addAction("Copy Address");

    this->_acthexdump->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));

    connect(this->_actcrossreferences, SIGNAL(triggered()), this, SLOT(onListingMenuCrossReferencesTriggered()));
    connect(this->_actrename, SIGNAL(triggered()), this, SLOT(renameSelectedListingObject()));
    connect(this->_acthexdump, SIGNAL(triggered()), this, SLOT(onListingMenuHexDumpTriggered()));
    connect(this->_actaddbookmark, SIGNAL(triggered()), this, SLOT(onListingMenuAddBookmarkTriggered()));
    connect(this->_actremovebookmark, SIGNAL(triggered()), this, SLOT(onListingMenuRemoveBookmarkTriggered()));
    connect(this->_actcopy, SIGNAL(triggered()), ui->disassemblerWidget, SLOT(copy()));
    connect(this->_actcopyaddress, SIGNAL(triggered()), ui->disassemblerWidget, SLOT(copyAddress()));
}

void DisassemblerView::createFunctionsMenu()
{
    this->_functionsmenu = new QMenu(this);
    QAction* actrename = this->_functionsmenu->addAction(QIcon(":/action_icons/res/rename.png"), "Rename");
    QAction* actjump = this->_functionsmenu->addAction(QIcon(":/action_icons/res/goto.png"), "Jump To Address");
    QAction* actxrefs = this->_functionsmenu->addAction(QIcon(":/misc_icons/res/crossreference.png"), "Cross References");

    connect(actrename, SIGNAL(triggered()), this, SLOT(renameSelectedFunction()));
    connect(actjump, SIGNAL(triggered()), this, SLOT(gotoFunction()));
    connect(actxrefs, SIGNAL(triggered()), this, SLOT(onFunctionsMenuXRefsTriggered()));
}

void DisassemblerView::createVariablesMenu()
{
    this->_variablesmenu = new QMenu(this);
    QAction* acycopyvariable = this->_variablesmenu->addAction(QIcon(":/action_icons/res/copy.png"), "Copy");
    QAction* actcopyname = this->_variablesmenu->addAction(QIcon(":/action_icons/res/copy.png"), "Copy Name");
    this->_variablesmenu->addSeparator();
    QAction* actrename = this->_variablesmenu->addAction(QIcon(":/action_icons/res/rename.png"), "Rename");
    QAction* actexportcsv = this->_variablesmenu->addAction(QIcon(":/action_icons/res/export.png"), "Export as CSV");

    connect(acycopyvariable, SIGNAL(triggered()), this, SLOT(copyVariable()));
    connect(actcopyname, SIGNAL(triggered()), this, SLOT(copyVariableName()));
    connect(actrename, SIGNAL(triggered()), this, SLOT(renameSelectedVariable()));
    connect(actexportcsv, SIGNAL(triggered()), this, SLOT(exportVariables()));
}

void DisassemblerView::createStringsMenu()
{
    this->_stringsmenu = new QMenu(this);
    QAction* actcopystring = this->_stringsmenu->addAction(QIcon(":/action_icons/res/copy.png"), "Copy");
    QAction* actcopystringonly = this->_stringsmenu->addAction(QIcon(":/action_icons/res/copy.png"), "Copy String");
    this->_stringsmenu->addSeparator();
    QAction* actexportcsv = this->_stringsmenu->addAction(QIcon(":/action_icons/res/export.png"), "Export as CSV");

    connect(actcopystring, SIGNAL(triggered()), this, SLOT(copyString()));
    connect(actcopystringonly, SIGNAL(triggered()), this, SLOT(copyStringOnly()));
    connect(actexportcsv, SIGNAL(triggered()), this, SLOT(exportStrings()));
}

void DisassemblerView::showCrossReference(Block *b)
{
    if(!b->hasSources())
        return;

    CrossReferenceDialog crd(b, this->_disassembler);
    int res = crd.exec();

    if(res == CrossReferenceDialog::Accepted && crd.selectedBlock())
        ui->disassemblerWidget->jumpTo(crd.selectedBlock());
}

void DisassemblerView::showCrossReference(const DataValue &address)
{
    Block* b = this->_listing->findBlock(address);

    if(!b || !b->hasSources())
        return;

    CrossReferenceDialog crd(b, this->_disassembler);
    int res = crd.exec();

    if(res == CrossReferenceDialog::Accepted && crd.selectedBlock())
        ui->disassemblerWidget->jumpTo(crd.selectedBlock());
}

void DisassemblerView::disassemble()
{
    if(!this->_hexeditdata)
        return;

    this->_worker = new DisassemblerWorker(this->_hexeditdata, this->_disassembler, ui->logWidget, this->_lblinfo, this->loadedFile(), this->_canloaddatabase, this);
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
        CrossReferenceDialog crd(reinterpret_cast<Function*>(index.internalPointer()), this->_disassembler);
        int res = crd.exec();

        if(res == CrossReferenceDialog::Accepted && crd.selectedBlock())
            ui->disassemblerWidget->jumpTo(crd.selectedBlock());
    }
}

void DisassemblerView::onListingMenuAboutToShow()
{
    Block* selectedblock = ui->disassemblerWidget->selectedBlock();

    this->_actrename->setVisible(selectedblock->blockType() != Block::InstructionBlock);
    this->_actcrossreferences->setVisible(selectedblock->hasSources());
    this->_actaddbookmark->setVisible(!selectedblock->isBookmarked());
    this->_actremovebookmark->setVisible(selectedblock->isBookmarked());
}

void DisassemblerView::onListingMenuAddBookmarkTriggered()
{
    ui->bookmarkWidget->setBlock(ui->disassemblerWidget->selectedBlock());
    ui->bookmarkWidget->show();

    ui->disassemblerWidget->update();
}

void DisassemblerView::onListingMenuRemoveBookmarkTriggered()
{
    this->_listing->toggleBookmark(ui->disassemblerWidget->selectedBlock());

    if(this->_listing->bookmarks().isEmpty())
        this->_actbookmarks->setEnabled(false);

    ui->disassemblerWidget->update();
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
            ui->dataView->selectRange(s->baseOffset(), s->size());
            break;
        }

        case Block::FunctionBlock:
        {
            Segment* s = this->_listing->findSegment(b);
            Function* f = qobject_cast<Function*>(b);
            ui->dataView->selectRange((f->startAddress() - s->startAddress()) + s->baseOffset(), f->size());
            break;
        }

        case Block::InstructionBlock:
        {
            Segment* s = this->_listing->findSegment(b);
            Instruction* i = qobject_cast<Instruction*>(b);
            ui->dataView->selectRange((i->startAddress() - s->startAddress()) + s->baseOffset(), i->size());
            break;
        }

        default:
            return;
    }

    ui->tabWidget->setCurrentIndex(1);
}

void DisassemblerView::renameBlock(Block *block)
{
    if(block->blockType() == Block::InstructionBlock)
        return;

    SymbolTable* symboltable = this->_listing->symbolTable();

    ui->renameWidget->setBlock(block);
    ui->renameWidget->setLabel(symboltable->name(block->startAddress()));
    ui->renameWidget->show();
}

void DisassemblerView::renameSelectedListingObject()
{
    this->renameBlock(ui->disassemblerWidget->selectedBlock());
}

void DisassemblerView::renameSelectedFunction()
{
    QItemSelectionModel* selectionmodel = ui->functionList->selectionModel();

    if(!selectionmodel->hasSelection())
        return;

    Block* block = reinterpret_cast<Block*>(selectionmodel->selectedIndexes()[0].internalPointer());
    this->renameBlock(block);
}

void DisassemblerView::renameSelectedVariable()
{
    QItemSelectionModel* selectionmodel = ui->tvVariables->selectionModel();

    if(!selectionmodel->hasSelection())
        return;

    Block* block = reinterpret_cast<Block*>(selectionmodel->selectedIndexes()[0].internalPointer());
    this->renameBlock(block);
}

void DisassemblerView::displayDisassembly()
{
    this->_listing = this->_disassembler->listing();
    this->_toolbar->setEnabled(true);

    ui->gotoWidget->setDisassembler(this->_disassembler);
    ui->disassemblerWidget->setDisassembler(this->_disassembler);

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

void DisassemblerView::showBookmarks()
{
    BookmarkDialog bd(this->_disassembler, this);
    int res = bd.exec();

    if((res == BookmarkDialog::Accepted) && bd.selectedBlock())
        ui->disassemblerWidget->jumpTo(bd.selectedBlock());
}

void DisassemblerView::loadDatabase()
{
    QString s = QFileDialog::getOpenFileName(this, "Load Database", QString(), "Disassembler Database (*.db)");

    if(!s.isEmpty())
    {
        DisassemblerDatabase dd(this->_listing->symbolTable(), s);

        this->_lblinfo->setText("Loading Database...");
        dd.load();
        this->_lblinfo->setText("Ready");
    }
}

void DisassemblerView::gotoFunction()
{
    QItemSelectionModel* selectionmodel = ui->functionList->selectionModel();

    if(selectionmodel->hasSelection())
        ui->disassemblerWidget->jumpTo(reinterpret_cast<Function*>(selectionmodel->selectedIndexes()[0].internalPointer()));
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

void DisassemblerView::copyStringOnly()
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

    this->showCrossReference(reinterpret_cast<Symbol*>(index.internalPointer()));
}

void DisassemblerView::on_tvVariables_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    this->showCrossReference(reinterpret_cast<Symbol*>(index.internalPointer()));
}

void DisassemblerView::on_tabOverview_currentChanged(int index)
{
    if(this->_worker->isRunning())
        return;

    switch(index)
    {
        case 1:
        {
            for(int i = 0; i < this->_variablesmodel->columnCount() - 1; i++)
                ui->tvVariables->resizeColumnToContents(i);

            ui->tvVariables->resizeRowsToContents();
            break;
        }

        case 2:
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

void DisassemblerView::onSaveBookmarkRequested(Block *block, const QString &description)
{
    this->_actbookmarks->setEnabled(true);
    this->_listing->applyBookmark(block, description);
}

void DisassemblerView::onRenameSymbolRequested(Block *block, const QString &symbolname)
{
    SymbolTable* symboltable = this->_listing->symbolTable();
    Symbol* symbol = symboltable->get(block->startAddress());

    symbol->setName(symbolname);
    ui->disassemblerWidget->update();
}
