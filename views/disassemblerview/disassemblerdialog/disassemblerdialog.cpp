#include "disassemblerdialog.h"
#include "ui_disassemblerdialog.h"

DisassemblerDialog::DisassemblerDialog(QHexEditData *hexeditdata, QWidget *parent): QDialog(parent), ui(new Ui::DisassemblerDialog)
{
    ui->setupUi(this);

    this->_loaderlistmodel = new LoaderListModel(hexeditdata, this);
    ui->loaderTable->setModel(this->_loaderlistmodel);
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
