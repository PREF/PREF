#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QHexEdit *hexedit, QWidget *parent): QDialog(parent), ui(new Ui::ExportDialog), _selexporter(nullptr), _hexedit(hexedit), _startoffset(0), _endoffset(0)
{
    ui->setupUi(this);

    connect(ui->pbCancel, SIGNAL(clicked()), this, SLOT(reject()));

    QHexEditData* hexeditdata = hexedit->data();
    this->_exportmodel = new ExporterModel();

    ui->pbExport->setEnabled(false);

    ui->sbbsFrom->setMinimum(0);
    ui->sbbsFrom->setMaximum(hexeditdata->length());

    ui->sbbsTo->setMinimum(1);
    ui->sbbsTo->setMaximum(hexeditdata->length());
    ui->sbbsTo->setValue(hexeditdata->length());

    ui->tvExporters->setModel(this->_exportmodel);

    if(hexedit->selectionStart() != hexedit->selectionEnd())
        ui->rbSelection->setChecked(true);
    else
        ui->rbAll->setChecked(true);

    for(int i = 0; i < this->_exportmodel->columnCount(); i++)
        ui->tvExporters->resizeColumnToContents(i);
}

void ExportDialog::setFixedRange(qint64 start, qint64 end)
{
    ui->sbbsFrom->setValue(start);
    ui->sbbsTo->setValue(end);

    ui->rbAll->setEnabled(false);
    ui->rbSelection->setEnabled(false);
    ui->rbRange->setChecked(true);
    ui->sbbsFrom->setEnabled(false);
    ui->sbbsTo->setEnabled(false);
}

ExporterDefinition *ExportDialog::selectedExporter() const
{
    return this->_selexporter;
}

const QString &ExportDialog::fileName() const
{
    return this->_filename;
}

quint64 ExportDialog::startOffset() const
{
    return qMin(this->_startoffset, this->_endoffset);
}

quint64 ExportDialog::endOffset() const
{
    return qMax(this->_startoffset, this->_endoffset);
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::validateFields()
{
    if(!this->_selexporter || ui->leFile->text().isEmpty())
        ui->pbExport->setEnabled(false);
    else
        ui->pbExport->setEnabled(true);
}

bool ExportDialog::queryRange()
{
    if(ui->rbAll->isChecked())
    {
        this->_startoffset = 0;
        this->_endoffset = this->_hexedit->data()->length();
        return true;
    }
    else if(ui->rbSelection->isChecked())
    {
        this->_startoffset = qMin(this->_hexedit->selectionStart(), this->_hexedit->selectionEnd());
        this->_endoffset = qMax(this->_hexedit->selectionStart(), this->_hexedit->selectionEnd());
        return true;
    }
    else if(ui->rbRange->isChecked())
    {
        this->_startoffset = qMin(ui->sbbsFrom->value(), ui->sbbsTo->value());
        this->_endoffset = qMax(ui->sbbsFrom->value(), ui->sbbsTo->value());
        return true;
    }

    return false;
}

void ExportDialog::on_rbRange_toggled(bool checked)
{
    if(checked)
    {
        ui->sbbsFrom->setEnabled(true);
        ui->sbbsTo->setEnabled(true);
    }
    else
    {
        ui->sbbsFrom->setEnabled(false);
        ui->sbbsTo->setEnabled(false);
    }
}

void ExportDialog::on_tbBrowse_clicked()
{
    QString s = QFileDialog::getSaveFileName(this, "Export...");

    if(!s.isEmpty())
    {
        this->_filename = s;
        ui->leFile->setText(s);
    }

    this->validateFields();
}

void ExportDialog::on_tvExporters_clicked(const QModelIndex &index)
{
    if(index.isValid())
        this->_selexporter = PrefContext::instance()->exporters()->get(index.row());
    else
    {
        this->_selexporter = nullptr;
        return;
    }

    this->validateFields();
}

void ExportDialog::on_pbExport_clicked()
{
    if(!this->queryRange())
    {
        QMessageBox mb;
        mb.setWindowTitle("Export Error");
        mb.setText("Invalid Export Parameters.\nExport Aborted.");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();

        return;
    }

    this->accept();
}
