#include "formatoptionsdialog.h"
#include "ui_formatoptionsdialog.h"

FormatOptionsDialog::FormatOptionsDialog(FormatDefinition::Ptr formatdef, FormatTree::Ptr formattree, ByteBuffer* bytebuffer, QWidget *parent): QDialog(parent), ui(new Ui::FormatOptionsDialog), _formatdefinition(formatdef), _formattree(formattree), _bytebuffer(bytebuffer), _selindex(-1)
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
    this->_formatdefinition->executeOption(this->_selindex, this->_formattree, this->_bytebuffer);
}

void FormatOptionsDialog::on_tvOptions_doubleClicked(const QModelIndex &index)
{
    this->_formatdefinition->executeOption(index.row() + 1, this->_formattree, this->_bytebuffer);
}
