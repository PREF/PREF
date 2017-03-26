#include "binaryview.h"
#include "ui_binaryview.h"

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
    this->updateStatus();
    this->analyze();
}

BinaryView::~BinaryView()
{
    delete ui;
}

void BinaryView::updateToolBar(QToolBar* toolbar) const
{
    toolbar->addAction(QIcon(":/res/save.png"), tr("Save"));
    connect(toolbar->addAction(QIcon(":/res/entropy.png"), tr("Map View")), SIGNAL(triggered()), ui->binaryNavigator, SLOT(switchView()));
    toolbar->addSeparator();
    toolbar->addAction(QIcon(":/res/undo.png"), tr("Undo"));
    toolbar->addAction(QIcon(":/res/redo.png"), tr("Redo"));
    toolbar->addSeparator();
    toolbar->addAction(QIcon(":/res/cut.png"), tr("Cut"));
    toolbar->addAction(QIcon(":/res/copy.png"), tr("Copy"));
    toolbar->addAction(QIcon(":/res/paste.png"), tr("Paste"));
    toolbar->addAction(QIcon(":/res/selectall.png"), tr("Select All"));
    toolbar->addSeparator();
    toolbar->addAction(QIcon(":/res/find.png"), tr("Find"));
    toolbar->addAction(QIcon(":/res/goto.png"), tr("Goto"));
}

void BinaryView::analyze()
{
    this->_datainspectormodel = new DataInspectorModel(ui->hexEdit);

    ui->chartTab->initialize(ui->hexEdit->data());
    ui->stringsTab->initialize(ui->hexEdit->data());
    ui->binaryNavigator->initialize(ui->hexEdit, this->_loadeddata);
    ui->visualMap->initialize(ui->hexEdit);
    ui->dataInspector->setModel(this->_datainspectormodel);
}

void BinaryView::updateStatus() const
{
    this->_lblstatus->setText(QString("<b>Offset:</b> %1h").arg(QString::number(ui->hexEdit->cursorPos(), 16).toUpper()));
}
