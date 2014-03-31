#include "formatoptionsdialog.h"
#include "ui_formatoptionsdialog.h"

FormatOptionsDialog::FormatOptionsDialog(const FormatDefinition *formatdefinition, QHexEditData *hexeditdata, QWidget *parent): QDialog(parent), ui(new Ui::FormatOptionsDialog), _formatdefinition(formatdefinition), _hexeditdata(hexeditdata), _selindex(-1)
{
    ui->setupUi(this);
    ui->tvOptions->setModel(new OptionModel(this->_formatdefinition));
}

FormatOptionsDialog::~FormatOptionsDialog()
{
    delete ui;
}

void FormatOptionsDialog::on_tvOptions_clicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        ui->pbExecute->setEnabled(false);
        return;
    }

    this->_selindex = index.row() + 1;
}

void FormatOptionsDialog::on_pbExecute_clicked()
{
    this->_formatdefinition->ExecuteOption(this->_selindex, this->_hexeditdata);
}

void FormatOptionsDialog::on_tvOptions_doubleClicked(const QModelIndex &index)
{
    this->_formatdefinition->ExecuteOption(index.row() + 1, this->_hexeditdata);
}
