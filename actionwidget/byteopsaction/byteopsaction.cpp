#include "byteopsaction.h"
#include "ui_byteopsaction.h"

ByteOpsAction::ByteOpsAction(QWidget *parent): QWidget(parent), ui(new Ui::ByteOpsAction)
{
    ui->setupUi(this);
    ui->sbbsValue->setMaximum(255);

    ui->cbByteOps->addItem("And", ByteOpsAction::And);         // 0
    ui->cbByteOps->addItem("Or", ByteOpsAction::Or);           // 1
    ui->cbByteOps->addItem("Xor", ByteOpsAction::Xor);         // 2
    ui->cbByteOps->addItem("Not", ByteOpsAction::Not);         // 3
    ui->cbByteOps->addItem("Mod", ByteOpsAction::Mod);         // 4

    this->setVisible(false);

    connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(hide()));
}

ByteOpsAction::~ByteOpsAction()
{
    delete ui;
}

void ByteOpsAction::on_cbByteOps_currentIndexChanged(int index)
{
    if(index == 3 || index == 5 || index == 6)
        ui->sbbsValue->setEnabled(false);
    else
        ui->sbbsValue->setEnabled(true);
}

void ByteOpsAction::on_pbExecute_clicked()
{
    uchar value = 0;
    int index = ui->cbByteOps->currentIndex();

    if(index != 3 && index != 5 && index != 6)
        value = (uchar)ui->sbbsValue->value();

    ByteOpsAction::ByteOperations bo = (ByteOpsAction::ByteOperations)ui->cbByteOps->itemData(ui->cbByteOps->currentIndex()).toInt();
    emit byteOperationRequested(value, bo);
}

void ByteOpsAction::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    ui->cbByteOps->setCurrentIndex(0);
    ui->sbbsValue->clear();
}
