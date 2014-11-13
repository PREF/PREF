#include "findaction.h"
#include "ui_findaction.h"

FindAction::FindAction(QWidget *parent): AbstractAction(parent), ui(new Ui::FindAction)
{
    ui->setupUi(this);

    ui->cbBase->addItem("ASCII String", QNumberLineEdit::AsciiString);
    //ui->cbBase->addItem("Unicode String", QNumberLineEdit::UnicodeString); Unicode not Supported
    ui->cbBase->addItem("Hex", QNumberLineEdit::HexString);

    this->setVisible(false);
    connect(ui->pbClose, SIGNAL(clicked()), this, SIGNAL(closeRequested()));
}

FindAction::~FindAction()
{
    delete ui;
}

void FindAction::on_cbBase_currentIndexChanged(int index)
{
    QNumberLineEdit::InputType it = (QNumberLineEdit::InputType)ui->cbBase->itemData(index).toInt();
    ui->leText->setInputType(it);
}

void FindAction::on_pbFind_clicked()
{
    if(ui->leText->text().length())
        emit findRequest(ui->leText->convertedData());
}

void FindAction::on_leText_returnPressed()
{
    if(ui->leText->text().length())
        emit findRequest(ui->leText->convertedData());
}

void FindAction::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    ui->cbBase->setCurrentIndex(0);
    ui->leText->clear();
}
