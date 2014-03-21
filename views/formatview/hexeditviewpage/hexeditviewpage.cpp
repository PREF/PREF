#include "hexeditviewpage.h"
#include "ui_hexeditviewpage.h"

HexEditViewPage::HexEditViewPage(ByteBuffer *bytebuffer, QWidget *parent): QWidget(parent), ui(new Ui::HexEditViewPage), _formatmodel(nullptr), _bytebuffer(bytebuffer), _toolbar(nullptr), _signscanenabled(false), _entropyenabled(false)
{
    ui->setupUi(this);

    ui->hSplitter->setStretchFactor(1, 1);
    ui->vSplitter->setStretchFactor(0, 1);
    ui->dataView->setData(bytebuffer);
    ui->hexEdit->setData(bytebuffer->hexEditData());

    this->_signaturecolor = QColor(0xFF, 0x8C, 0x8C);

    this->updateOffset(0);
    this->updateSelLength(0);
    this->createToolBar();

    ui->binaryNavigator->setData(ui->hexEdit, bytebuffer);
    ui->tvFormat->setModel(new FormatModel(FormatTree::Ptr(), bytebuffer)); /* Empty Model */

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

bool HexEditViewPage::loadFormat(const FormatDefinition::Ptr &fd, qint64 baseoffset)
{
    this->_bytebuffer->setBaseOffset(baseoffset);
    bool validated = fd->validateFormat(this->_bytebuffer, baseoffset);

    if(validated)
    {
        this->_formatdefinition = fd;
        this->_formatmodel = fd->parseFormat(this->_bytebuffer, baseoffset);
        ui->tvFormat->setModel(this->_formatmodel);
    }
    else
        PrefDebug::dbgprint(QString("\nFormat Validation Failed for '%1'").arg(fd->name()));

    this->_bytebuffer->setBaseOffset(0);
    return validated;
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

FormatTree::Ptr HexEditViewPage::tree()
{
    return this->_formatmodel->tree();
}

FormatDefinition::Ptr HexEditViewPage::formatDefinition()
{
    return this->_formatdefinition;
}

void HexEditViewPage::gotoOffset(qint64 offset, qint64 length)
{
    if(length == -1)
        length = offset;

    ui->hexEdit->setSelection(offset, offset + length);
}

void HexEditViewPage::createToolBar()
{
    this->_toolbar = new ActionToolBar(ui->hexEdit, this->_bytebuffer, ui->tbContainer);
    this->_toolbar->createActions(ui->actionWidget, ActionToolBar::AllActions);

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

void HexEditViewPage::onHexEditCustomContextMenuRequested(const QPoint &pos)
{
    QPoint newpos = ui->hexEdit->mapToGlobal(pos);
    this->_toolbar->actionMenu()->popup(newpos);
}

void HexEditViewPage::onSetBackColor(FormatElement *formatobj)
{
    QColor c = QColorDialog::getColor(Qt::white, this);

    if(c.isValid())
    {
        lua_Integer offset = formatobj->offset();
        ui->hexEdit->highlightBackground(offset, (offset + formatobj->size() - 1), c);
    }
}

void HexEditViewPage::onRemoveBackColor(FormatElement *formatobj)
{
    lua_Integer offset = formatobj->offset();
    ui->hexEdit->clearHighlight(offset, (offset + formatobj->size() - 1));
}

void HexEditViewPage::onFormatObjectSelected(FormatElement *formatobj)
{
    lua_Integer offset = formatobj->offset();
    ui->hexEdit->setSelection(offset, offset + formatobj->size());
}

void HexEditViewPage::exportData(FormatElement *formatobj)
{
    ExportDialog ed(ui->hexEdit, this->_bytebuffer, this);
    ed.setFixedRange(formatobj->offset(), formatobj->endOffset());
    ed.exec();
}

void HexEditViewPage::importData(FormatElement *formatobj)
{
    QString s = QFileDialog::getOpenFileName(this, "Import binary file...");

    if(!s.isEmpty())
    {
        QFile f(s);
        f.open(QIODevice::ReadOnly);

        qint64 offset = formatobj->offset();
        qint64 size = qMin(f.size(), (formatobj->endOffset() - offset));

        if (size > 0)
        {
            QByteArray ba = f.read(size);
            this->_bytebuffer->write(offset, size, ba);
        }
    }
}

void HexEditViewPage::scanSignatures()
{
    if(!this->_signscanenabled)
        return;

    QByteArray foundsig;
    qint64 sigid, depth = 0, offset = ui->hexEdit->visibleStartOffset(), endoffset = ui->hexEdit->visibleEndOffset();

    while(offset <= endoffset)
    {
        bool stepdone = SignatureDatabase::database()->step(this->_bytebuffer->at(offset), depth, sigid, foundsig);

        /* We have found a good byte, check if it is a dead path */
        if(stepdone && !SignatureDatabase::database()->canContinue(this->_bytebuffer->at(offset + 1), depth))
        {
            depth = 0;
            foundsig.clear();
            offset++;
            continue;
        }

        /* We have completed a path, verify depth and if it is valid, highlight it in QHexEdit */
        if(!stepdone && (depth > 0) && (offset < endoffset) && !SignatureDatabase::database()->canContinue(this->_bytebuffer->at(offset + 1), depth))
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
