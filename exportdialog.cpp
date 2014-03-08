#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QHexEdit *hexedit, ByteBuffer* bytebuffer, QWidget *parent): QDialog(parent), ui(new Ui::ExportDialog), _state(bytebuffer->state()), _hexedit(hexedit), _bytebuffer(bytebuffer)
{
    ui->setupUi(this);

    connect(ui->pbCancel, SIGNAL(clicked()), this, SLOT(reject()));

    this->_hexedit = hexedit;
    this->_exportmodel = new ExportModel();

    ExportList::load(this->_state);

    ui->pbExport->setEnabled(false);

    ui->sbbsFrom->setMinimum(0);
    ui->sbbsFrom->setMaximum(bytebuffer->length());

    ui->sbbsTo->setMinimum(1);
    ui->sbbsTo->setMaximum(bytebuffer->length());
    ui->sbbsTo->setValue(bytebuffer->length());

    ui->tvExporters->setModel(this->_exportmodel);

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

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::validateFields()
{
    if(!this->_selexporter || !ui->leFile->text().length())
        ui->pbExport->setEnabled(false);
    else
        ui->pbExport->setEnabled(true);
}

bool ExportDialog::queryRange(lua_Integer &from, lua_Integer &to)
{
    if(ui->rbAll->isChecked())
    {
        from = 0;
        to = this->_bytebuffer->length();
        return true;
    }
    else if(ui->rbSelection->isChecked())
    {
        from = qMin(this->_hexedit->selectionStart(), this->_hexedit->selectionEnd());
        to = qMax(this->_hexedit->selectionStart(), this->_hexedit->selectionEnd());
        return true;
    }
    else if(ui->rbRange->isChecked())
    {
        from = qMin(ui->sbbsFrom->value(), ui->sbbsTo->value());
        to = qMax(ui->sbbsFrom->value(), ui->sbbsTo->value());
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
        this->_selexporter = ExportList::exporter(index.row());
    else
        this->_selexporter = ExportDefinition::Ptr();

    this->validateFields();
}

void ExportDialog::on_pbExport_clicked()
{
    lua_Integer from, to;

    if(!this->queryRange(from, to))
    {
        QMessageBox mb;
        mb.setWindowTitle("Export Error");
        mb.setText("Invalid Export Parameters.\nExport Aborted.");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();

        return;
    }

    this->_selexporter->exportData(this->_bytebuffer, this->_filename, from, to);
    this->accept();
}
