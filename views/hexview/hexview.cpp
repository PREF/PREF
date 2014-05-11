#include "hexview.h"
#include "ui_hexview.h"

HexView::HexView(QHexEditData* hexeditdata, QLabel *labelinfo, QWidget *parent): AbstractView(labelinfo, parent), ui(new Ui::HexView), _formattree(nullptr), _hexeditdata(hexeditdata), _toolbar(nullptr), _signscanenabled(false), _entropyenabled(false)
{
    ui->setupUi(this);

    ui->hSplitter->setStretchFactor(0, 1);
    ui->vSplitter->setStretchFactor(0, 1);
    ui->dataView->setData(hexeditdata);
    ui->hexEdit->setData(hexeditdata);

    this->_signaturecolor = QColor(0xFF, 0x8C, 0x8C);
    this->_formatmodel = new FormatModel(hexeditdata);
    ui->tvFormat->setModel(this->_formatmodel);

    this->createToolBar();
    this->inspectData(hexeditdata);

    connect(ui->hexEdit, SIGNAL(positionChanged(qint64)), ui->dataView->model(), SLOT(setOffset(qint64)));
    connect(ui->hexEdit, SIGNAL(positionChanged(qint64)), this, SLOT(updateOffset(qint64)));
    connect(ui->hexEdit, SIGNAL(selectionChanged(qint64)), this, SLOT(updateSelLength(qint64)));
    connect(ui->hexEdit, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onHexEditCustomContextMenuRequested(QPoint)));
    connect(ui->hexEdit, SIGNAL(verticalScrollBarValueChanged(int)), ui->binaryNavigator, SLOT(renderMap(int)));

    connect(ui->tvFormat, SIGNAL(setBackColor(FormatElement*)), this, SLOT(onSetBackColor(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(removeBackColor(FormatElement*)), this, SLOT(onRemoveBackColor(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(formatObjectSelected(FormatElement*)), this, SLOT(onFormatObjectSelected(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(exportAction(FormatElement*)), this, SLOT(exportData(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(importAction(FormatElement*)), this, SLOT(importData(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(gotoOffset(qint64)), ui->hexEdit, SLOT(setCursorPos(qint64)));
}

bool HexView::loadFormat(FormatList::Format &format, int64_t baseoffset)
{
    this->_formattree = SDKManager::parseFormat(format.id(), baseoffset, this->_hexeditdata);
    this->_formatmodel->setFormatTree(this->_formattree);

    this->_tbformatoptions->setMenu(new OptionMenu(SDKManager::state(), ui->hexEdit, format));
    this->_tbformatoptions->setEnabled(format.optionsCount() > 0);

    for(int i = 0; i < this->_formatmodel->columnCount(); i++)
        ui->tvFormat->resizeColumnToContents(i);

    return !this->_formattree->isEmpty();
}

void HexView::save()
{
    this->_hexeditdata->save();
}

void HexView::save(QString filename)
{
    QFile f(filename);
    this->_hexeditdata->saveTo(&f);
    f.close();
}

HexView::~HexView()
{
    delete ui;
}

bool HexView::canSave() const
{
    return true;
}

void HexView::updateStatusBar()
{
    QString offset = QString("%1").arg(ui->hexEdit->cursorPos(), ui->hexEdit->addressWidth(), 16, QLatin1Char('0')).toUpper();
    QString size = QString("%1").arg(ui->hexEdit->selectionLength(), ui->hexEdit->addressWidth(), 16, QLatin1Char('0')).toUpper();
    this->updateInfoText(QString("<b>Offset:</b> %1h&nbsp;&nbsp;&nbsp;&nbsp;<b>Size:</b> %2h").arg(offset, size));
}

void HexView::closeEvent(QCloseEvent *event)
{
    FormatList::removeLoadedFormat(this->_hexeditdata);
    AbstractView::closeEvent(event);
}

void HexView::createToolBar()
{
    this->_toolbar = new ActionToolBar(ui->hexEdit, ui->tbContainer);
    this->_tbloadformat = new QToolButton();
    this->_tbformatoptions = new QToolButton();
    this->_tbbyteview = new QToolButton();

    this->_tbloadformat->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->_tbloadformat->setIcon(QIcon(":/misc_icons/res/format.png"));
    this->_tbloadformat->setText("Formats");

    this->_tbformatoptions->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->_tbformatoptions->setIcon(QIcon(":/misc_icons/res/formatoptions.png"));
    this->_tbformatoptions->setText("Options");
    this->_tbformatoptions->setPopupMode(QToolButton::MenuButtonPopup);
    this->_tbformatoptions->setEnabled(false);

    this->_tbbyteview->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->_tbbyteview->setIcon(QIcon(":/action_icons/res/entropy.png"));
    this->_tbbyteview->setText("Map View");
    this->_tbbyteview->setCheckable(true);

    this->_toolbar->addWidget(this->_tbloadformat);
    this->_toolbar->addWidget(this->_tbformatoptions);
    this->_toolbar->addWidget(this->_tbbyteview);
    this->_toolbar->addSeparator();
    this->_toolbar->createActions(ui->actionWidget, ActionToolBar::AllActions);

    connect(this->_tbloadformat, SIGNAL(clicked()), this, SLOT(onLoadFormatClicked()));
    connect(this->_tbbyteview, SIGNAL(clicked()), this, SLOT(onByteViewClicked()));

    QVBoxLayout* vl = new QVBoxLayout();
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(this->_toolbar);

    ui->tbContainer->setLayout(vl);
}

void HexView::inspectData(QHexEditData *hexeditdata)
{
    ui->binaryNavigator->setData(ui->hexEdit);
    ui->chartWidget->plot(hexeditdata);
    ui->stringsWidget->scan(hexeditdata);
    ui->signaturesWidget->scan(hexeditdata);

    connect(ui->stringsWidget, SIGNAL(gotoTriggered(qint64,qint64)), ui->hexEdit, SLOT(setSelectionRange(qint64,qint64)));
    connect(ui->signaturesWidget, SIGNAL(gotoTriggered(qint64,qint64)), ui->hexEdit, SLOT(setSelectionRange(qint64,qint64)));
}

void HexView::updateOffset(qint64)
{
    this->updateStatusBar();
}

void HexView::updateSelLength(qint64 size)
{
    this->updateStatusBar();

    if(!this->_toolbar)
        return;

    if(!size)
    {
        this->_toolbar->setEditActionsEnabled(false);
        ui->actionWidget->byteOpsAction()->hide();
    }
    else
        this->_toolbar->setEditActionsEnabled(true);
}

void HexView::onLoadFormatClicked()
{
    FormatsDialog fd(this->_hexeditdata->length(), this->topLevelWidget());
    int res = fd.exec();

    if(res == FormatsDialog::Accepted)
    {
        this->_formatid = fd.selectedFormat();
        FormatList::Format& format = FormatList::formatFromId(this->_formatid);

        if(this->loadFormat(format, fd.offset()))
        {
            ui->tabWidget->setCurrentIndex(2); /* Select Format Page */
            FormatList::addLoadedFormat(this->_formatid, this->_formattree, this->_hexeditdata);

            if(format.canDisassemble())
            {
                //NOTE: Can Disassemble...
            }
        }
    }
}

void HexView::onByteViewClicked()
{
    if(this->_tbbyteview->isChecked())
        ui->binaryNavigator->displayEntropy();
    else
        ui->binaryNavigator->displayDefault();
}

void HexView::onHexEditCustomContextMenuRequested(const QPoint &pos)
{
    QPoint newpos = ui->hexEdit->mapToGlobal(pos);
    this->_toolbar->actionMenu()->popup(newpos);
}

void HexView::onSetBackColor(FormatElement *formatelement)
{
    QColor c = QColorDialog::getColor(Qt::white, this);

    if(c.isValid())
    {
        uint64_t offset = formatelement->offset();
        ui->hexEdit->highlightBackground(offset, (offset + formatelement->size() - 1), c);
    }
}

void HexView::onRemoveBackColor(FormatElement *formatelement)
{
    uint64_t offset = formatelement->offset();
    ui->hexEdit->clearHighlight(offset, (offset + formatelement->size() - 1));
}

void HexView::onFormatObjectSelected(FormatElement *formatelement)
{
    uint64_t offset = formatelement->offset();
    ui->hexEdit->setSelection(offset, offset + formatelement->size());
}

void HexView::exportData(FormatElement *formatelement)
{
    ExportDialog ed(ui->hexEdit, this);
    ed.setFixedRange(formatelement->offset(), formatelement->endOffset());
    int res = ed.exec();

    if(res == ExportDialog::Accepted)
        ExporterList::exportData(ed.selectedExporter().id(), ed.fileName(), this->_hexeditdata, ed.startOffset(), ed.endOffset());
}

void HexView::importData(FormatElement *formatelement)
{
    QString s = QFileDialog::getOpenFileName(this, "Import binary file...");

    if(!s.isEmpty())
    {
        QFile f(s);
        f.open(QIODevice::ReadOnly);

        uint64_t offset = formatelement->offset();
        uint64_t size = qMin(static_cast<uint64_t>(f.size()), (formatelement->endOffset() - offset));

        if (size > 0)
        {
            QByteArray ba = f.read(size);
            QHexEditDataWriter writer(this->_hexeditdata);
            writer.replace(offset, size, ba);
        }
    }
}
