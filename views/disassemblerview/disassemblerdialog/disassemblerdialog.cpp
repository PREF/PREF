#include "disassemblerdialog.h"
#include "ui_disassemblerdialog.h"

DisassemblerDialog::DisassemblerDialog(QHexEditData *hexeditdata, QWidget *parent): QDialog(parent), ui(new Ui::DisassemblerDialog)
{
    ui->setupUi(this);

    this->_disassemblerlistmodel = new DisassemblerListModel(hexeditdata, this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    ui->disassemblerTable->setModel(this->_disassemblerlistmodel);
    ui->disassemblerTable->resizeRowsToContents();
    ui->disassemblerTable->resizeColumnToContents(0);
}

bool DisassemblerDialog::elaborateInstructions() const
{
    return ui->cbElaborateInstructions->isChecked();
}

bool DisassemblerDialog::analyzeListing() const
{
    return ui->cbElaborateInstructions->isChecked() && ui->cbAnalyzeListing->isChecked();
}

bool DisassemblerDialog::hasLoaders() const
{
    return this->_disassemblerlistmodel->rowCount() > 0;
}

DisassemblerDefinition* DisassemblerDialog::selectedDisassembler() const
{
    QItemSelectionModel* selectionmodel = ui->disassemblerTable->selectionModel();
    QModelIndexList selectedindexes = selectionmodel->selectedRows();

    if(selectedindexes.length())
        return this->_disassemblerlistmodel->disassembler(selectedindexes[0].row());

    return nullptr;
}

DisassemblerDialog::~DisassemblerDialog()
{
    delete ui;
}

void DisassemblerDialog::on_disassemblerTable_doubleClicked(const QModelIndex&)
{
    this->accept();
}

void DisassemblerDialog::on_disassemblerTable_clicked(const QModelIndex &index)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(index.isValid());
}

void DisassemblerDialog::on_cbElaborateInstructions_stateChanged(int state)
{
    ui->cbAnalyzeListing->setEnabled(((state == Qt::Checked) ? true : false));
}
