#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent): QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setFixedSize(520, 330);

    ui->twComponents->setColumnCount(2);
    ui->twComponents->setRowCount(2);

    ui->twComponents->setItem(0, 0, new QTableWidgetItem("SDK Version"));
    ui->twComponents->setItem(0, 1, new QTableWidgetItem(SDKManager::sdkVersion()));
    ui->twComponents->setItem(1, 0, new QTableWidgetItem("Lua Version"));
    ui->twComponents->setItem(1, 1, new QTableWidgetItem(QString(LUA_RELEASE)));

    ui->twComponents->resizeRowsToContents();
    ui->twComponents->resizeColumnsToContents();
    ui->twDevs->resizeRowsToContents();
    ui->twDevs->resizeColumnsToContents();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
