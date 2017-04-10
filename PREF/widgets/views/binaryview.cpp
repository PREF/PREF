#include "binaryview.h"
#include "ui_binaryview.h"
#include <QFileDialog>

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
    ui->hexEdit->setDocument(document);

    connect(ui->hexEdit->document()->cursor(), &QHexCursor::positionChanged, this, &BinaryView::updateStatus);
    connect(ui->hexEdit->document()->cursor(), &QHexCursor::selectionChanged, this, &BinaryView::updateStatus);

    this->updateStatus();
    this->analyze();
}

BinaryView::~BinaryView()
{
    delete ui;
}

void BinaryView::updateToolBar(QToolBar* toolbar) const
{
    QHexDocument* document = ui->hexEdit->document();
    QHexCursor* cursor = document->cursor();

    toolbar->addAction(QIcon(":/res/save.png"), tr("Save"))->setEnabled(!ui->hexEdit->readOnly());
    toolbar->addAction(QIcon(":/res/entropy.png"), tr("Map View"), ui->binaryNavigator, &BinaryNavigator::switchView);
    toolbar->addAction(QIcon(":/res/template.png"), tr("Load Template"), this, &BinaryView::loadTemplate);
    toolbar->addSeparator();
    toolbar->addAction(QIcon(":/res/undo.png"), tr("Undo"), document, &QHexDocument::undo);
    toolbar->addAction(QIcon(":/res/redo.png"), tr("Redo"), document, &QHexDocument::redo);
    toolbar->addSeparator();
    QAction* actcut = toolbar->addAction(QIcon(":/res/cut.png"), tr("Cut"), document, &QHexDocument::cut);
    QAction* actcopy = toolbar->addAction(QIcon(":/res/copy.png"), tr("Copy"), document, &QHexDocument::copy);
    QAction* actpaste = toolbar->addAction(QIcon(":/res/paste.png"), tr("Paste"), document, &QHexDocument::paste);
    toolbar->addAction(QIcon(":/res/selectall.png"), tr("Select All"), cursor, &QHexCursor::selectAll);
    toolbar->addSeparator();
    toolbar->addAction(QIcon(":/res/find.png"), tr("Find"));
    toolbar->addAction(QIcon(":/res/goto.png"), tr("Goto"));

    actcut->setEnabled(false);
    actcopy->setEnabled(false);
    actpaste->setEnabled(false);

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

void BinaryView::on_tvTemplate_clicked(const QModelIndex &index)
{
    if(!index.isValid() || !index.internalPointer())
        return;

    QHexCursor* cursor = ui->hexEdit->document()->cursor();
    BTEntry* btentry = reinterpret_cast<BTEntry*>(index.internalPointer());
    cursor->setSelectionRange(btentry->location.offset, btentry->location.size);
}
