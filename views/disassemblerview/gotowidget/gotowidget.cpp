#include "gotowidget.h"
#include "ui_gotowidget.h"

GotoWidget::GotoWidget(QWidget *parent): QWidget(parent), ui(new Ui::GotoWidget), _disassembler(nullptr)
{
    ui->setupUi(this);
    ui->leAddress->setInputType(QNumberLineEdit::HexString);
    this->setVisible(false);

    connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ui->pbGoto, SIGNAL(clicked()), this, SLOT(gotoBlock()));
    connect(ui->leAddress, SIGNAL(returnPressed()), this, SLOT(gotoBlock()));
}

GotoWidget::~GotoWidget()
{
    delete ui;
}

void GotoWidget::setDisassembler(DisassemblerDefinition* disassembler)
{
    /* FIXME:
    this->_disassembler = disassembler;
    ui->leAddress->setMaxLength(DataType::byteWidth(disassembler->addressType())); */
}

QLineEdit *GotoWidget::addressWidget()
{
    return ui->leAddress;
}

void GotoWidget::gotoBlock()
{
    /* FIXME:
    DataType::Type addresstype = this->_disassembler->addressType();

    if(static_cast<uint>(ui->leAddress->text().length()) != DataType::byteWidth(addresstype))
        return;

    bool ok = false;
    quint64 addr = ui->leAddress->text().toULongLong(&ok, 16);

    if(!ok)
        return;

    emit addressRequested(DataValue::create(addr, addresstype));
    */
}

void GotoWidget::hideEvent(QHideEvent* e)
{
    ui->leAddress->clear();
    QWidget::hideEvent(e);
}

void GotoWidget::showEvent(QShowEvent* e)
{
    ui->leAddress->setFocus();
    QWidget::showEvent(e);
}

void GotoWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
        this->hide();

    QWidget::keyPressEvent(e);
}
