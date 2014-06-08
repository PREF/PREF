#include "disassemblerdialog.h"
#include "ui_disassemblerdialog.h"

DisassemblerDialog::DisassemblerDialog(QHexEditData *hexeditdata, QWidget *parent): QDialog(parent), ui(new Ui::DisassemblerDialog), _selectedloader(nullptr)
{
    ui->setupUi(this);

    this->_loaderlistmodel = new LoaderListModel(hexeditdata, this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    ui->loaderTable->setModel(this->_loaderlistmodel);
    ui->loaderTable->resizeRowsToContents();
    ui->loaderTable->resizeColumnToContents(0);
}

bool DisassemblerDialog::hasLoaders() const
{
    return this->_loaderlistmodel->rowCount() > 0;
}

LoaderList::LoaderId DisassemblerDialog::selectedLoader() const
{
    QItemSelectionModel* selectionmodel = ui->loaderTable->selectionModel();
    QModelIndexList selectedindexes = selectionmodel->selectedRows();

    if(selectedindexes.length())
        return this->_loaderlistmodel->loader(selectedindexes[0].row());

    return nullptr;
}

DisassemblerDialog::~DisassemblerDialog()
{
    delete ui;
}

void DisassemblerDialog::on_loaderTable_doubleClicked(const QModelIndex&)
{
    this->accept();
}

void DisassemblerDialog::on_loaderTable_clicked(const QModelIndex &index)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(index.isValid());
}
