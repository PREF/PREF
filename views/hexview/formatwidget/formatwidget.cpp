#include "formatwidget.h"
#include "ui_formatwidget.h"

FormatWidget::FormatWidget(QWidget *parent): WorkerTab(parent), ui(new Ui::FormatWidget), _startoffset(0), _formatmodel(nullptr), _hexedit(nullptr), _logwidget(nullptr)
{
    ui->setupUi(this);

    connect(&this->_validatorwatcher, SIGNAL(finished()), this, SLOT(onValidationFinished()));
    connect(&this->_parsewatcher, SIGNAL(finished()), this, SLOT(onParseFinished()));
}

void FormatWidget::setLogWidget(LogWidget *logwidget)
{
    this->_logwidget = logwidget;
}

FormatWidget::~FormatWidget()
{
    delete ui;
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

void FormatWidget::resetData()
{
    this->_formatmodel = new FormatModel(this->_hexedit->data(), this);
    ui->tvFormat->setModel(this->_formatmodel);
}

void FormatWidget::loadFormat()
{
    FormatsDialog fd(this->_hexedit->data()->length(), (this->_hexedit->selectionLength() ? this->_hexedit->cursorPos() : 0), this->topLevelWidget());
    int res = fd.exec();

    if(res == FormatsDialog::Accepted)
    {
        this->_formatdefinition = fd.selectedFormat();
        this->_startoffset = fd.offset();

        emit workStarted();
        QFuture<bool> future = QtConcurrent::run(this->_formatdefinition, &FormatDefinition::callValidate, this->_hexedit->data(), this->_startoffset, false);
        this->_validatorwatcher.setFuture(future);
    }
}

void FormatWidget::onSetBackColor(FormatElement *formatelement)
{
    QColor c = QColorDialog::getColor(Qt::white, this);

    if(c.isValid())
    {
        quint64 offset = formatelement->offset();
        this->_hexedit->highlightBackground(offset, (offset + formatelement->size() - 1), c);
    }
}

void FormatWidget::onRemoveBackColor(FormatElement *formatelement)
{
    quint64 offset = formatelement->offset();
    this->_hexedit->clearHighlight(offset, (offset + formatelement->size() - 1));
}

void FormatWidget::onFormatObjectSelected(FormatElement *formatelement)
{
    quint64 offset = formatelement->offset();
    this->_hexedit->setSelection(offset, offset + formatelement->size());
}

void FormatWidget::exportData(FormatElement *formatelement)
{
    ExportDialog ed(this->_hexedit, this);
    ed.setFixedRange(formatelement->offset(), formatelement->endOffset());
    int res = ed.exec();

    if(res == ExportDialog::Accepted)
        ed.selectedExporter()->callDump(this->_hexedit->data(), ed.fileName(), ed.startOffset(), ed.endOffset());
}

void FormatWidget::importData(FormatElement *formatelement)
{
    QString s = QFileDialog::getOpenFileName(this, "Import binary file...");

    if(!s.isEmpty())
    {
        QFile f(s);
        f.open(QIODevice::ReadOnly);

        quint64 size = qMin(static_cast<quint64>(f.size()), formatelement->size());

        if(size > 0)
        {
            QByteArray ba = f.read(size);
            QHexEditDataWriter writer(this->_hexedit->data());
            writer.replace(formatelement->offset(), size, ba);
        }
    }
}

void FormatWidget::onValidationFinished()
{
    bool validated = this->_validatorwatcher.result();

    if(!validated)
    {
        emit workFinished();
        return;
    }

    emit parseStarted();
    QFuture<FormatTree*> future = QtConcurrent::run(this->_formatdefinition, &FormatDefinition::callParse, this->_hexedit->data(), this->_logwidget, this->_startoffset);
    this->_parsewatcher.setFuture(future);
}

void FormatWidget::onParseFinished()
{
    QWidget* formatview = nullptr;
    FormatTree* formattree = this->_parsewatcher.future().result();

    if(formattree && !formattree->isEmpty())
    {
        this->_formatmodel->setFormatTree(formattree);

        for(int i = 0; i < this->_formatmodel->columnCount(); i++)
            ui->tvFormat->resizeColumnToContents(i);

        if(this->_formatdefinition->hasView())
            formatview = this->_formatdefinition->callView(this->_hexedit->data(), formattree);
    }

    emit parseFinished(formattree, formatview);
    emit workFinished();
}
