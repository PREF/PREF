#include "entrypointsdialog.h"
#include "ui_entrypointsdialog.h"

EntryPointsDialog::EntryPointsDialog(DisassemblerListing* listing, QWidget *parent): QDialog(parent), ui(new Ui::EntryPointsDialog)
{
    ui->setupUi(this);
    ui->entryPointsTable->setModel(new EntryPointsModel(listing, ui->entryPointsTable));
    ui->entryPointsTable->resizeRowsToContents();
}

EntryPointsDialog::~EntryPointsDialog()
{
    delete ui;
}
