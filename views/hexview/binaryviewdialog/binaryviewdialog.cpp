#include "binaryviewdialog.h"
#include "ui_binaryviewdialog.h"

BinaryViewDialog::BinaryViewDialog(QHexEditData *hexeditdata, QWidget *parent): QDialog(parent), ui(new Ui::BinaryViewDialog), _ctrlpressed(false), _shiftpressed(false)
{
    ui->setupUi(this);

    ui->sbStep->setMinimum(1);
    ui->sbStep->setValue(0x10);
    ui->sbStep->setMaximum(hexeditdata->length());

    ui->binaryMap->setStart(0);
    ui->binaryMap->setEnd(hexeditdata->length());
    ui->binaryMap->setWidth(256);
    ui->binaryMap->setStep(ui->sbStep->value());
    ui->binaryMap->setDisplayMode(BinaryMap::DotPlot);
    ui->binaryMap->setData(hexeditdata);

    connect(ui->binaryMap, SIGNAL(gotoTriggered(qint64)), this, SIGNAL(gotoTriggered(qint64)));
    connect(ui->binaryMap, SIGNAL(ctrlModifier(bool)), this, SLOT(onCtrlModifier(bool)));
    connect(ui->binaryMap, SIGNAL(shiftModifier(bool)), this, SLOT(onShiftModifier(bool)));
    connect(ui->binaryMap, SIGNAL(offsetChanged(QString)), ui->lblCursorOffset, SLOT(setText(QString)));
    connect(ui->sbStep, SIGNAL(valueChanged(qint64)), ui->binaryMap, SLOT(setStep(qint64)));
    connect(ui->displayModeWidget, SIGNAL(displayModeChanged(BinaryMap::DisplayMode)), ui->binaryMap, SLOT(setDisplayMode(BinaryMap::DisplayMode)));
}

BinaryViewDialog::~BinaryViewDialog()
{
    delete ui;
}

void BinaryViewDialog::onCtrlModifier(bool pressed)
{
    if(this->_ctrlpressed != pressed)
    {
        this->_ctrlpressed = pressed;

        QFont f(qApp->font());
        f.setBold(pressed);

        ui->lblCtrl->setFont(f);
    }
}

void BinaryViewDialog::onShiftModifier(bool pressed)
{
    if(this->_shiftpressed != pressed)
    {
        this->_shiftpressed = pressed;

        QFont f(qApp->font());
        f.setBold(pressed);

        ui->lblShift->setFont(f);
    }
}

void BinaryViewDialog::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

void BinaryViewDialog::keyPressEvent(QKeyEvent *event)
{
    ui->binaryMap->keyPressEvent(event); /* Forward */

    QWidget::keyPressEvent(event);
}

void BinaryViewDialog::keyReleaseEvent(QKeyEvent *event)
{
    ui->binaryMap->keyReleaseEvent(event);  /* Forward */

    QWidget::keyReleaseEvent(event);
}
