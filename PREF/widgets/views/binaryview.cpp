#include "binaryview.h"
#include "ui_binaryview.h"

#define pad(s, w) s += QString("&nbsp;").repeated(w)

BinaryView::BinaryView(QHexEditData *hexeditdata, QLabel *lblstatus, const QString &loadedfile, QWidget *parent): AbstractView(hexeditdata, lblstatus, loadedfile, parent), ui(new Ui::BinaryView)
{
    ui->setupUi(this);

    ui->vSplitter->setSizes(QList<int>() << (this->height() * 0.60)
                                         << (this->height() * 0.40));

    ui->hSplitter->setSizes(QList<int>() << (this->width() * 0.75)
                                         << (this->width() * 0.25));

    this->_loadeddata = new LoadedData(hexeditdata);
    ui->hexEdit->setData(hexeditdata);

    connect(ui->hexEdit, &QHexEdit::positionChanged, [this](qint64) { this->updateStatus(); });
    connect(ui->hexEdit, &QHexEdit::selectionChanged, [this](qint64) { this->updateStatus(); });
    this->updateStatus();
    this->analyze();
}

BinaryView::~BinaryView()
{
    delete ui;
}

void BinaryView::updateToolBar(QToolBar* toolbar) const
{
    toolbar->addAction(QIcon(":/res/save.png"), tr("Save"))->setEnabled(!ui->hexEdit->readOnly());
    toolbar->addAction(QIcon(":/res/entropy.png"), tr("Map View"), ui->binaryNavigator, &BinaryNavigator::switchView);
    toolbar->addAction(QIcon(":/res/template.png"), tr("Load Template"), this, &BinaryView::loadTemplate);
    toolbar->addSeparator();
    toolbar->addAction(QIcon(":/res/undo.png"), tr("Undo"), ui->hexEdit, &QHexEdit::undo);
    toolbar->addAction(QIcon(":/res/redo.png"), tr("Redo"), ui->hexEdit, &QHexEdit::redo);
    toolbar->addSeparator();
    QAction* actcut = toolbar->addAction(QIcon(":/res/cut.png"), tr("Cut"), ui->hexEdit, &QHexEdit::cut);
    QAction* actcopy = toolbar->addAction(QIcon(":/res/copy.png"), tr("Copy"), ui->hexEdit, &QHexEdit::copy);
    QAction* actpaste = toolbar->addAction(QIcon(":/res/paste.png"), tr("Paste"), ui->hexEdit, &QHexEdit::paste);
    toolbar->addAction(QIcon(":/res/selectall.png"), tr("Select All"), ui->hexEdit, &QHexEdit::selectAll);
    toolbar->addSeparator();
    toolbar->addAction(QIcon(":/res/find.png"), tr("Find"));
    toolbar->addAction(QIcon(":/res/goto.png"), tr("Goto"));

    actcut->setEnabled(false);
    actcopy->setEnabled(false);
    actpaste->setEnabled(false);

    connect(ui->hexEdit, &QHexEdit::selectionChanged, [this, actcut, actcopy, actpaste](qint64 length) {
        actcut->setEnabled(length > 0);
        actcopy->setEnabled(length > 0);
        actpaste->setEnabled(length > 0);
    });
}

void BinaryView::analyze()
{
    this->_datainspectormodel = new DataInspectorModel(ui->hexEdit);
    this->_templatemodel = new TemplateModel(ui->hexEdit);

    ui->chartTab->initialize(ui->hexEdit->data());
    ui->stringsTab->initialize(ui->hexEdit->data());
    ui->binaryNavigator->initialize(ui->hexEdit, this->_loadeddata);
    ui->visualMap->initialize(ui->hexEdit);
    ui->dataInspector->setModel(this->_datainspectormodel);
    ui->tvTemplate->setModel(this->_templatemodel);
}

void BinaryView::updateStatus() const
{
    QString info = QString("<b>Offset:</b> %1 [%2h]").arg(QString::number(ui->hexEdit->cursorPos()))
                                                     .arg(QString::number(ui->hexEdit->cursorPos(), 16).toUpper());

    pad(info, 10);
    info += QString("<b>Size:</b> %1 [%2h]").arg(QString::number(ui->hexEdit->selectionLength()))
                                            .arg(QString::number(ui->hexEdit->selectionLength(), 16).toUpper());

    this->_lblstatus->setText(info);
}

void BinaryView::loadTemplate()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Load template..."), QString(), "010 Editor Template (*.bt)");

    if(file.isEmpty())
        return;

    this->_templatemodel->execute(file);
}

void BinaryView::on_tvTemplate_clicked(const QModelIndex &index)
{
    if(!index.isValid() || !index.internalPointer())
        return;

    BTEntry* btentry = reinterpret_cast<BTEntry*>(index.internalPointer());
    ui->hexEdit->setSelection(btentry->location.offset, btentry->location.end());
}
