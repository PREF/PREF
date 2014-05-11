#include "binaryviewdialog.h"
#include "ui_binaryviewdialog.h"

BinaryViewDialog::BinaryViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BinaryViewDialog)
{
    ui->setupUi(this);
}

BinaryViewDialog::~BinaryViewDialog()
{
    delete ui;
}
