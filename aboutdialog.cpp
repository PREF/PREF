#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent): QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setFixedSize(520, 330);

    QTreeWidgetItem* itemluaver = new QTreeWidgetItem((QTreeWidgetItem*)NULL, QStringList() << "LUA Version" << QString(LUA_RELEASE));
    QTreeWidgetItem* itemsdkversion = new QTreeWidgetItem((QTreeWidgetItem*)NULL, QStringList() << "SDK Version" << SDKManager::version());

    ui->listPrefInfo->addTopLevelItem(itemluaver);
    ui->listPrefInfo->addTopLevelItem(itemsdkversion);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
