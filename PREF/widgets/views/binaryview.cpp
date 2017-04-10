#include "binaryview.h"
#include "ui_binaryview.h"
#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>

#define pad(s, w) s += QString("&nbsp;").repeated(w)

BinaryView::BinaryView(QHexDocument *document, QLabel *lblstatus, const QString &loadedfile, QWidget *parent): AbstractView(document, lblstatus, loadedfile, parent), ui(new Ui::BinaryView)
{
    ui->setupUi(this);

    ui->vSplitter->setSizes(QList<int>() << (this->height() * 0.60)
                                         << (this->height() * 0.40));

    ui->hSplitter->setSizes(QList<int>() << (this->width() * 0.75)
                                         << (this->width() * 0.25));

    ui->tvTemplate->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    this->_loadeddata = new LoadedData(document);
    this->_menu = new QMenu(this);

    ui->hexEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->hexEdit->setDocument(document);

    connect(ui->hexEdit, &QHexEdit::customContextMenuRequested, [this](const QPoint&) { this->_menu->popup(QCursor::pos()); });
    connect(ui->hexEdit->document()->cursor(), &QHexCursor::positionChanged, this, &BinaryView::updateStatus);
    connect(ui->hexEdit->document()->cursor(), &QHexCursor::selectionChanged, this, &BinaryView::updateStatus);

    this->initSaveMenu();
    this->updateStatus();
    this->analyze();
}

BinaryView::~BinaryView()
{
    delete ui;
}

void BinaryView::initSaveMenu()
{
    this->_savemenu = new QMenu(this);
    this->_savemenu->addAction(tr("Save"), this, &BinaryView::save)->setShortcut(QKeySequence(Qt::Key_F2));
    this->_savemenu->addAction(tr("Save As"), this, &BinaryView::saveAs)->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F2));
}

void BinaryView::updateToolBar(QToolBar* toolbar) const
{
    QHexDocument* document = ui->hexEdit->document();
    QHexCursor* cursor = document->cursor();

    QAction* actsave = toolbar->addAction(QIcon(":/res/save.png"), tr("Save"), this, &BinaryView::save);
    actsave->setEnabled(!ui->hexEdit->readOnly());
    actsave->setMenu(this->_savemenu);

    QToolButton* toolbutton = static_cast<QToolButton*>(toolbar->widgetForAction(actsave));
    toolbutton->setPopupMode(QToolButton::MenuButtonPopup);

    toolbar->addAction(QIcon(":/res/entropy.png"), tr("Map View"), ui->binaryNavigator, &BinaryNavigator::switchView);
    toolbar->addAction(QIcon(":/res/template.png"), tr("Load Template"), this, &BinaryView::loadTemplate);
    toolbar->addSeparator();
    QAction* actundo = toolbar->addAction(QIcon(":/res/undo.png"), tr("Undo"), document, &QHexDocument::undo);
    QAction* actredo = toolbar->addAction(QIcon(":/res/redo.png"), tr("Redo"), document, &QHexDocument::redo);
    toolbar->addSeparator();
    QAction* actcut = toolbar->addAction(QIcon(":/res/cut.png"), tr("Cut"), document, &QHexDocument::cut);
    QAction* actcopy = toolbar->addAction(QIcon(":/res/copy.png"), tr("Copy"), document, &QHexDocument::copy);
    QAction* actpaste = toolbar->addAction(QIcon(":/res/paste.png"), tr("Paste"), document, &QHexDocument::paste);
    QAction* actselectall = toolbar->addAction(QIcon(":/res/selectall.png"), tr("Select All"), cursor, &QHexCursor::selectAll);
    toolbar->addSeparator();
    QAction* actfind = toolbar->addAction(QIcon(":/res/find.png"), tr("Find"));
    QAction* actgoto = toolbar->addAction(QIcon(":/res/goto.png"), tr("Goto"));

    actundo->setEnabled(false);
    actredo->setEnabled(false);
    actcut->setEnabled(false);
    actcopy->setEnabled(false);
    actpaste->setEnabled(false);

    this->_menu->addAction(actundo);
    this->_menu->addAction(actredo);
    this->_menu->addSeparator();
    this->_menu->addAction(actcut);
    this->_menu->addAction(actcopy);
    this->_menu->addAction(actpaste);
    this->_menu->addSeparator();
    this->_menu->addAction(actselectall);
    this->_menu->addAction(actfind);
    this->_menu->addAction(actgoto);

    connect(this->_document, &QHexDocument::canUndoChanged, [this, actundo]() { actundo->setEnabled(this->_document->canUndo());});
    connect(this->_document, &QHexDocument::canRedoChanged, [this, actredo]() { actredo->setEnabled(this->_document->canRedo());});

    connect(cursor, &QHexCursor::selectionChanged, [cursor, actcut, actcopy, actpaste]() {
        actcut->setEnabled(cursor->selectionLength() > 0);
        actcopy->setEnabled(cursor->selectionLength() > 0);
        actpaste->setEnabled(cursor->selectionLength() > 0);
    });
}

void BinaryView::analyze()
{
    this->_datainspectormodel = new DataInspectorModel(ui->hexEdit);
    this->_templatemodel = new TemplateModel(ui->hexEdit);

    ui->chartTab->initialize(ui->hexEdit->document());
    ui->stringsTab->initialize(ui->hexEdit->document());
    ui->binaryNavigator->initialize(ui->hexEdit, this->_loadeddata);
    ui->visualMap->initialize(ui->hexEdit);
    ui->dataInspector->setModel(this->_datainspectormodel);
    ui->tvTemplate->setModel(this->_templatemodel);
}

void BinaryView::saveTo(QFile *f)
{
    if(!f->isOpen())
        f->open(QFile::WriteOnly);

    if(!this->_document->saveTo(f))
        QMessageBox::warning(this, tr("Save failed"), tr("Cannot save '%1'").arg(this->loadedFile()));

    f->close();
}

void BinaryView::on_tvTemplate_clicked(const QModelIndex &index)
{
    if(!index.isValid() || !index.internalPointer())
        return;

    QHexCursor* cursor = ui->hexEdit->document()->cursor();
    BTEntry* btentry = reinterpret_cast<BTEntry*>(index.internalPointer());
    cursor->setSelectionRange(btentry->location.offset, btentry->location.size);
}

void BinaryView::updateStatus() const
{
    QHexCursor* cursor = ui->hexEdit->document()->cursor();

    QString info = QString("<b>Offset:</b> %1 [%2h]").arg(QString::number(cursor->offset()))
                                                     .arg(QString::number(cursor->offset(), 16).toUpper());

    pad(info, 10);
    info += QString("<b>Size:</b> %1 [%2h]").arg(QString::number(cursor->selectionLength()))
                                            .arg(QString::number(cursor->selectionLength(), 16).toUpper());

    this->_lblstatus->setText(info);
}

void BinaryView::loadTemplate()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Load template..."), QString(), "010 Editor Template (*.bt)");

    if(file.isEmpty())
        return;

    ui->hexEdit->document()->clearMetadata();
    this->_templatemodel->execute(file);
    ui->tabView->setCurrentIndex(2);
}

void BinaryView::save()
{
    QFile f(this->loadedFile());
    this->saveTo(&f);
}

void BinaryView::saveAs()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), "All files (*.*)");

    if(file.isEmpty())
        return;

    QFile f(file);
    this->saveTo(&f);
}
