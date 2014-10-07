#include "gotowidget.h"
#include "ui_gotowidget.h"

GotoWidget::GotoWidget(QWidget *parent): QWidget(parent), ui(new Ui::GotoWidget), _listing(nullptr)
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

void GotoWidget::setListing(DisassemblerListing *listing)
{
    if(!this->_listing)
        ui->leAddress->setMaxLength(DataType::byteWidth(listing->addressType()));

    this->_listing = listing;
}

QLineEdit *GotoWidget::addressWidget()
{
    return ui->leAddress;
}

void GotoWidget::gotoBlock()
{
    DataType::Type addresstype = this->_listing->addressType();

    if(static_cast<uint>(ui->leAddress->text().length()) != DataType::byteWidth(addresstype))
        return;

    bool ok = false;
    quint64 addr = ui->leAddress->text().toULongLong(&ok, 16);

    if(!ok)
        return;

    emit addressRequested(DataValue::create(addr, addresstype));
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
