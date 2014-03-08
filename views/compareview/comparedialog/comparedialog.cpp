#include "comparedialog.h"
#include "ui_comparedialog.h"

CompareDialog::CompareDialog(QWidget *parent): QDialog(parent), ui(new Ui::CompareDialog)
{
    ui->setupUi(this);
}

CompareDialog::~CompareDialog()
{
    delete ui;
}

QString CompareDialog::leftCompare() const
{
    return this->_left;
}

QString CompareDialog::rightCompare() const
{
    return this->_right;
}

QString CompareDialog::selectFile()
{
    return QFileDialog::getOpenFileName(this, "Load File...");
}

void CompareDialog::on_tbLeft_clicked()
{
    this->_left = this->selectFile();
    ui->leLeft->setText(this->_left);
}

void CompareDialog::on_tbRight_clicked()
{
    this->_right = this->selectFile();
    ui->leRight->setText(this->_right);
}
