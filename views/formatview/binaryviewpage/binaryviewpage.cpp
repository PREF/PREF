#include "binaryviewpage.h"
#include "ui_binaryviewpage.h"

BinaryViewPage::BinaryViewPage(ByteBuffer* bytebuffer, QWidget *parent): QWidget(parent), ui(new Ui::BinaryViewPage), _bytebuffer(bytebuffer), _ctrlpressed(false), _shiftpressed(false)
{
    ui->setupUi(this);

    ui->sbStep->setMinimum(1);
    ui->sbStep->setValue(0x10);
    ui->sbStep->setMaximum(bytebuffer->length());

    ui->binaryMap->setStart(0);
    ui->binaryMap->setEnd(bytebuffer->length());
    ui->binaryMap->setWidth(256);
    ui->binaryMap->setStep(ui->sbStep->value());
    ui->binaryMap->setDisplayMode(BinaryMap::DotPlot);
    ui->binaryMap->setData(bytebuffer);

    connect(ui->binaryMap, SIGNAL(gotoTriggered(qint64)), this, SIGNAL(gotoTriggered(qint64)));
    connect(ui->binaryMap, SIGNAL(ctrlModifier(bool)), this, SLOT(onCtrlModifier(bool)));
    connect(ui->binaryMap, SIGNAL(shiftModifier(bool)), this, SLOT(onShiftModifier(bool)));
    connect(ui->binaryMap, SIGNAL(offsetChanged(QString)), ui->lblCursorOffset, SLOT(setText(QString)));
    connect(ui->sbStep, SIGNAL(valueChanged(qint64)), ui->binaryMap, SLOT(setStep(qint64)));
    connect(ui->displayModeWidget, SIGNAL(displayModeChanged(BinaryMap::DisplayMode)), ui->binaryMap, SLOT(setDisplayMode(BinaryMap::DisplayMode)));
}

BinaryViewPage::~BinaryViewPage()
{
    delete ui;
}

void BinaryViewPage::onCtrlModifier(bool pressed)
{
    if(this->_ctrlpressed != pressed)
    {
        this->_ctrlpressed = pressed;

        QFont f(qApp->font());
        f.setBold(pressed);

        ui->lblCtrl->setFont(f);
    }
}

void BinaryViewPage::onShiftModifier(bool pressed)
{
    if(this->_shiftpressed != pressed)
    {
        this->_shiftpressed = pressed;

        QFont f(qApp->font());
        f.setBold(pressed);

        ui->lblShift->setFont(f);
    }
}

void BinaryViewPage::keyPressEvent(QKeyEvent *event)
{
    ui->binaryMap->keyPressEvent(event); /* Forward */

    QWidget::keyPressEvent(event);
}

void BinaryViewPage::keyReleaseEvent(QKeyEvent *event)
{
    ui->binaryMap->keyReleaseEvent(event);  /* Forward */

    QWidget::keyReleaseEvent(event);
}
