#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent): QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setFixedSize(520, 330);

    ui->twComponents->setColumnCount(2);
    ui->twComponents->setRowCount(3);

    ui->twComponents->setItem(0, 0, new QTableWidgetItem("SDK Version"));
    ui->twComponents->setItem(0, 1, new QTableWidgetItem(SDKManager::version()));
    ui->twComponents->setItem(1, 0, new QTableWidgetItem("Lua Version"));
    ui->twComponents->setItem(1, 1, new QTableWidgetItem(QString(LUA_RELEASE)));
    ui->twComponents->setItem(2, 0, new QTableWidgetItem("LuaJIT Version"));
    ui->twComponents->setItem(2, 1, new QTableWidgetItem(QString(LUAJIT_VERSION)));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
