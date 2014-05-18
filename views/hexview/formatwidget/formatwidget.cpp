#include "formatwidget.h"
#include "ui_formatwidget.h"

FormatWidget::FormatWidget(QWidget *parent): WorkerTab(parent), ui(new Ui::FormatWidget), _formatmodel(nullptr), _hexedit(nullptr), _formattree(nullptr)
{
    ui->setupUi(this);
}

FormatWidget::~FormatWidget()
{
    delete ui;
}

FormatTree* FormatWidget::parseFormat(FormatList::FormatId formatid, qint64 baseoffset)
{
    emit workStarted();

    lua_State* l = SDKManager::state();
    lua_State* thread = lua_newthread(l);
    FormatTree* formattree = new FormatTree(l, this->_hexedit->data(), baseoffset, this);

    lua_getglobal(thread, "Sdk");
    lua_getfield(thread, -1, "parseFormat");
    lua_pushstring(thread, formatid);
    lua_pushinteger(thread, static_cast<lua_Integer>(baseoffset));
    lua_pushlightuserdata(thread, this->_hexedit->data());
    lua_pushlightuserdata(thread, formattree);

    if(lua_resume(thread, 4))
        DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(thread, -1)));

    lua_pop(thread, 1);
    lua_pop(l, 1);

    emit workFinished();
    return formattree;
}

FormatTree *FormatWidget::tree()
{
    return this->_formattree;
}

void FormatWidget::setData(QHexEdit *hexedit)
{
    this->_hexedit = hexedit;
    this->_formatmodel = new FormatModel(this->_hexedit->data(), this);

    ui->tvFormat->setModel(this->_formatmodel);

    connect(ui->tvFormat, SIGNAL(setBackColor(FormatElement*)), this, SLOT(onSetBackColor(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(removeBackColor(FormatElement*)), this, SLOT(onRemoveBackColor(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(formatObjectSelected(FormatElement*)), this, SLOT(onFormatObjectSelected(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(exportAction(FormatElement*)), this, SLOT(exportData(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(importAction(FormatElement*)), this, SLOT(importData(FormatElement*)));
    connect(ui->tvFormat, SIGNAL(gotoOffset(qint64)), this->_hexedit, SLOT(setCursorPos(qint64)));
}

FormatList::FormatId FormatWidget::loadFormat()
{
    FormatsDialog fd(this->_hexedit->data()->length(), this->topLevelWidget());
    int res = fd.exec();

    if(res != FormatsDialog::Accepted)
        return nullptr;

    FormatList::FormatId formatid = fd.selectedFormat();
    this->_formattree = this->parseFormat(formatid, fd.offset());

    if(this->_formattree->isEmpty())
        return nullptr;

    this->_formatmodel->setFormatTree(this->_formattree);

    for(int i = 0; i < this->_formatmodel->columnCount(); i++)
        ui->tvFormat->resizeColumnToContents(i);

    return formatid;
}

void FormatWidget::onSetBackColor(FormatElement *formatelement)
{
    QColor c = QColorDialog::getColor(Qt::white, this);

    if(c.isValid())
    {
        uint64_t offset = formatelement->offset();
        this->_hexedit->highlightBackground(offset, (offset + formatelement->size() - 1), c);
    }
}

void FormatWidget::onRemoveBackColor(FormatElement *formatelement)
{
    uint64_t offset = formatelement->offset();
    this->_hexedit->clearHighlight(offset, (offset + formatelement->size() - 1));
}

void FormatWidget::onFormatObjectSelected(FormatElement *formatelement)
{
    uint64_t offset = formatelement->offset();
    this->_hexedit->setSelection(offset, offset + formatelement->size());
}

void FormatWidget::exportData(FormatElement *formatelement)
{
    ExportDialog ed(this->_hexedit, this);
    ed.setFixedRange(formatelement->offset(), formatelement->endOffset());
    int res = ed.exec();

    if(res == ExportDialog::Accepted)
        ExporterList::exportData(ed.selectedExporter().id(), ed.fileName(), this->_hexedit->data(), ed.startOffset(), ed.endOffset());
}

void FormatWidget::importData(FormatElement *formatelement)
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
            QHexEditDataWriter writer(this->_hexedit->data());
            writer.replace(offset, size, ba);
        }
    }
}
