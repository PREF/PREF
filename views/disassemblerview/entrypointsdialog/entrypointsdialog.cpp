#include "entrypointsdialog.h"
#include "ui_entrypointsdialog.h"

EntryPointsDialog::EntryPointsDialog(DisassemblerListing* listing, QWidget *parent): QDialog(parent), ui(new Ui::EntryPointsDialog), _selectedfunction(nullptr)
{
    ui->setupUi(this);
    ui->entryPointsTable->setModel(new EntryPointsModel(listing, ui->entryPointsTable));
    ui->entryPointsTable->resizeRowsToContents();
}

Function *EntryPointsDialog::selectedFunction() const
{
    return this->_selectedfunction;
}

EntryPointsDialog::~EntryPointsDialog()
{
    delete ui;
}

void EntryPointsDialog::on_entryPointsTable_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        this->_selectedfunction = reinterpret_cast<Function*>(index.internalPointer());
        this->done(EntryPointsDialog::Accepted);
    }
    else
        this->done(EntryPointsDialog::Rejected);
}

void EntryPointsDialog::on_buttonBox_accepted()
{
    QItemSelectionModel* model = ui->entryPointsTable->selectionModel();
    QModelIndex index = model->currentIndex();

    if(index.isValid())
        this->_selectedfunction = reinterpret_cast<Function*>(index.internalPointer());
}
