#include "hexeditviewpage.h"
#include "ui_hexeditviewpage.h"

HexEditViewPage::HexEditViewPage(QHexEditData* hexeditdata, QWidget *parent): QWidget(parent), ui(new Ui::HexEditViewPage), _formattree(nullptr), _hexeditdata(hexeditdata), _toolbar(nullptr), _signscanenabled(false), _entropyenabled(false)
{
    ui->setupUi(this);

    ui->hSplitter->setStretchFactor(0, 1);
    ui->vSplitter->setStretchFactor(0, 1);
    ui->dataView->setData(hexeditdata);
    ui->hexEdit->setData(hexeditdata);

    this->_signaturecolor = QColor(0xFF, 0x8C, 0x8C);

    this->updateOffset(0);
    this->updateSelLength(0);
    this->createToolBar();

    this->_formatmodel = new FormatModel(hexeditdata);
    ui->tvFormat->setModel(this->_formatmodel);
    ui->binaryNavigator->setData(ui->hexEdit);

    connect(ui->hexEdit, SIGNAL(positionChanged(qint64)), ui->dataView->model(), SLOT(setOffset(qint64)));
    connect(ui->hexEdit, SIGNAL(positionChanged(qint64)), this, SLOT(updateOffset(qint64)));
    connect(ui->hexEdit, SIGNAL(selectionChanged(qint64)), this, SLOT(updateSelLength(qint64)));
    connect(ui->hexEdit, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onHexEditCustomContextMenuRequested(QPoint)));
    connect(ui->hexEdit, SIGNAL(verticalScrollBarValueChanged(int)), ui->binaryNavigator, SLOT(renderMap(int)));
    connect(ui->hexEdit, SIGNAL(visibleLinesChanged()), this, SLOT(scanSignatures()));

    connect(ui->tvFormat, SIGNAL(setBackColor(FormatElement*)), this, SLOT(onSetBackColor(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(removeBackColor(FormatElement*)), this, SLOT(onRemoveBackColor(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(formatObjectSelected(FormatElement*)), this, SLOT(onFormatObjectSelected(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(exportAction(FormatElement*)), this, SLOT(exportData(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(importAction(FormatElement*)), this, SLOT(importData(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(gotoOffset(qint64)), ui->hexEdit, SLOT(setCursorPos(qint64)));
}

bool HexEditViewPage::loadFormat(FormatList::Format& format, int64_t baseoffset)
{
    this->_formattree = SDKManager::parseFormat(format.id(), baseoffset, this->_hexeditdata);
    this->_formatmodel->setFormatTree(this->_formattree);

    this->_tbformatoptions->setMenu(new OptionMenu(SDKManager::state(), ui->hexEdit, format));
    this->_tbformatoptions->setEnabled(format.optionsCount() > 0);

    for(int i = 0; i < this->_formatmodel->columnCount(); i++)
        ui->tvFormat->resizeColumnToContents(i);

    return !this->_formattree->isEmpty();
}

void HexEditViewPage::scanSignatures(bool b)
{
    this->_signscanenabled = b;

    if(this->_signscanenabled)
        this->scanSignatures();
    else
    {
        ui->hexEdit->clearHighlight();
        ui->hexEdit->clearComments();
    }
}

BinaryNavigator *HexEditViewPage::binaryNavigator()
{
    return ui->binaryNavigator;
}

FormatTree *HexEditViewPage::tree()
{
    return this->_formattree;
}

void HexEditViewPage::gotoOffset(qint64 offset, qint64 length)
{
    if(length == -1)
        length = offset;

    ui->hexEdit->setSelection(offset, offset + length);
}

void HexEditViewPage::createToolBar()
{
    this->_toolbar = new ActionToolBar(ui->hexEdit, ui->tbContainer);
    this->_tbloadformat = new QToolButton();
    this->_tbformatoptions = new QToolButton();
    this->_tbbyteview = new QToolButton();
    this->_tbscansignature = new QToolButton();

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

    this->_tbscansignature->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->_tbscansignature->setIcon(QIcon(":/misc_icons/res/signature.png"));
    this->_tbscansignature->setText("Signatures");
    this->_tbscansignature->setCheckable(true);

    this->_toolbar->addWidget(this->_tbloadformat);
    this->_toolbar->addWidget(this->_tbformatoptions);
    this->_toolbar->addWidget(this->_tbbyteview);
    this->_toolbar->addWidget(this->_tbscansignature);
    this->_toolbar->addSeparator();
    this->_toolbar->createActions(ui->actionWidget, ActionToolBar::AllActions);

    connect(this->_tbloadformat, SIGNAL(clicked()), this, SLOT(onLoadFormatClicked()));
    connect(this->_tbformatoptions, SIGNAL(clicked()), this, SLOT(onFormatOptionsClicked()));
    connect(this->_tbbyteview, SIGNAL(clicked()), this, SLOT(onByteViewClicked()));
    connect(this->_tbscansignature, SIGNAL(clicked()), this, SLOT(onSignatureScannerClicked()));

    QVBoxLayout* vl = new QVBoxLayout();
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(this->_toolbar);

    ui->tbContainer->setLayout(vl);
}

QHexEdit *HexEditViewPage::hexEdit()
{
    return ui->hexEdit;
}

HexEditViewPage::~HexEditViewPage()
{

}

void HexEditViewPage::updateOffset(qint64 offset)
{
    ui->lblOffset->setText("Offset: 0x" + QString("%1").arg(offset, ui->hexEdit->addressWidth(), 16, QLatin1Char('0')).toUpper() + "  ");
    ui->lblOffset->setFixedWidth(ui->lblOffset->fontMetrics().width(ui->lblOffset->text()));
}

void HexEditViewPage::updateSelLength(qint64 size)
{
    ui->lblSize->setText("Size: 0x" + QString("%1").arg(size, ui->hexEdit->addressWidth(), 16, QLatin1Char('0')).toUpper());

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

void HexEditViewPage::onLoadFormatClicked()
{
    FormatsDialog fd(this->_hexeditdata->length(), this->topLevelWidget());
    int res = fd.exec();

    if(res == FormatsDialog::Accepted)
    {
        this->_formatid = fd.selectedFormat();
        FormatList::Format& format = FormatList::formatFromId(this->_formatid);

        if(this->loadFormat(format, fd.offset()))
        {
            FormatList::addLoadedFormat(this->_formatid, this->_formattree, this->_hexeditdata);
            emit formatLoaded(this->_formatid);
        }
    }
}

void HexEditViewPage::onFormatOptionsClicked()
{
    //FormatOptionsDialog fod(SDKManager::state(), this->_formatid, ui->hexEdit, this->topLevelWidget());
    //fod.exec();
}

void HexEditViewPage::onSignatureScannerClicked()
{
    this->scanSignatures(this->_tbscansignature->isChecked());
}

void HexEditViewPage::onByteViewClicked()
{
    if(this->_tbbyteview->isChecked())
        ui->binaryNavigator->displayEntropy();
    else
        ui->binaryNavigator->displayDefault();
}

void HexEditViewPage::onHexEditCustomContextMenuRequested(const QPoint &pos)
{
    QPoint newpos = ui->hexEdit->mapToGlobal(pos);
    this->_toolbar->actionMenu()->popup(newpos);
}

void HexEditViewPage::onSetBackColor(FormatElement* formatelement)
{
    QColor c = QColorDialog::getColor(Qt::white, this);

    if(c.isValid())
    {
        uint64_t offset = formatelement->offset();
        ui->hexEdit->highlightBackground(offset, (offset + formatelement->size() - 1), c);
    }
}

void HexEditViewPage::onRemoveBackColor(FormatElement* formatelement)
{
    uint64_t offset = formatelement->offset();
    ui->hexEdit->clearHighlight(offset, (offset + formatelement->size() - 1));
}

void HexEditViewPage::onFormatObjectSelected(FormatElement* formatelement)
{
    uint64_t offset = formatelement->offset();
    ui->hexEdit->setSelection(offset, offset + formatelement->size());
}

void HexEditViewPage::exportData(FormatElement *formatelement)
{
    ExportDialog ed(ui->hexEdit, this);
    ed.setFixedRange(formatelement->offset(), formatelement->endOffset());
    int res = ed.exec();

    if(res == ExportDialog::Accepted)
        ExporterList::exportData(ed.selectedExporter().id(), ed.fileName(), this->_hexeditdata, ed.startOffset(), ed.endOffset());
}

void HexEditViewPage::importData(FormatElement* formatelement)
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

void HexEditViewPage::scanSignatures()
{
    if(!this->_signscanenabled)
        return;

    QByteArray foundsig;
    QHexEditDataReader reader(this->_hexeditdata);
    qint64 sigid, depth = 0, offset = ui->hexEdit->visibleStartOffset(), endoffset = ui->hexEdit->visibleEndOffset();

    while(offset <= endoffset)
    {
        bool stepdone = SignatureDatabase::database()->step(reader.at(offset), depth, sigid, foundsig);

        /* We have found a good byte, check if it is a dead path */
        if(stepdone && !SignatureDatabase::database()->canContinue(reader.at(offset + 1), depth))
        {
            depth = 0;
            foundsig.clear();
            offset++;
            continue;
        }

        /* We have completed a path, verify depth and if it is valid, highlight it in QHexEdit */
        if(!stepdone && (depth > 0) && (offset < endoffset) && !SignatureDatabase::database()->canContinue(reader.at(offset + 1), depth))
        {
            /* Check False Positives: ignore it and continue the analysis */
            if(depth != SignatureDatabase::database()->signatureMaxDepth(sigid) || !SignatureDatabase::database()->isSignatureValid(foundsig, sigid))
            {
                depth = 0;
                foundsig.clear();
                offset += depth;
                continue;
            }

            qint64 olddepth = depth;
            depth = 0;

            if(olddepth > 1)
            {
                qint64 startoffset = (offset - olddepth) + 1;

                ui->hexEdit->highlightBackground(startoffset, offset, this->_signaturecolor);
                ui->hexEdit->commentRange(startoffset, offset, SignatureDatabase::database()->signatureName(sigid));

                offset += olddepth;
                continue;
            }
        }

        offset++;
    }
}
