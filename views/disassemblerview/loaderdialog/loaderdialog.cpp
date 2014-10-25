#include "loaderdialog.h"
#include "ui_loaderdialog.h"

LoaderDialog::LoaderDialog(QHexEditData *hexeditdata, QWidget *parent): QDialog(parent), ui(new Ui::LoaderDialog)
{
    ui->setupUi(this);

    this->_loaderlistmodel = new LoaderListModel(hexeditdata, this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    ui->loaderTable->setModel(this->_loaderlistmodel);
    ui->loaderTable->resizeRowsToContents();
    ui->loaderTable->resizeColumnToContents(0);
}

bool LoaderDialog::elaborateInstructions() const
{
    return ui->cbElaborateInstructions->isChecked();
}

bool LoaderDialog::analyzeListing() const
{
    return ui->cbElaborateInstructions->isChecked() && ui->cbAnalyzeListing->isChecked();
}

bool LoaderDialog::hasLoaders() const
{
    return this->_loaderlistmodel->rowCount() > 0;
}

ProcessorLoader* LoaderDialog::selectedLoader() const
{
    QItemSelectionModel* selectionmodel = ui->loaderTable->selectionModel();
    QModelIndexList selectedindexes = selectionmodel->selectedRows();

    if(selectedindexes.length())
        return this->_loaderlistmodel->loader(selectedindexes[0].row());

    return nullptr;
}

LoaderDialog::~LoaderDialog()
{
    delete ui;
}

void LoaderDialog::on_loaderTable_doubleClicked(const QModelIndex&)
{
    this->accept();
}

void LoaderDialog::on_loaderTable_clicked(const QModelIndex &index)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(index.isValid());
}

void LoaderDialog::on_cbElaborateInstructions_stateChanged(int state)
{
    ui->cbAnalyzeListing->setEnabled(((state == Qt::Checked) ? true : false));
}
