#include "spinboxbaseselector.h"
#include "ui_spinboxbaseselector.h"

SpinBoxBaseSelector::SpinBoxBaseSelector(QWidget *parent) : QWidget(parent), ui(new Ui::SpinBoxBaseSelector)
{
    ui->setupUi(this);

    ui->cbBase->addItem("Hex", 16); // 0
    ui->cbBase->addItem("Dec", 10); // 1
    ui->cbBase->addItem("Bin", 2);  // 2
    ui->cbBase->addItem("Oct", 8);  // 3
}

void SpinBoxBaseSelector::setMinimum(qint64 val)
{
    ui->sbValue->setMinimum(val);
}

void SpinBoxBaseSelector::setMaximum(qint64 val)
{
    ui->sbValue->setMaximum(val);
}

void SpinBoxBaseSelector::setBase(int b)
{
    ui->sbValue->setBase(b);

    switch(b)
    {
        case 2:
            ui->cbBase->setCurrentIndex(2);
            break;

        case 8:
            ui->cbBase->setCurrentIndex(3);
            break;

        case 10:
            ui->cbBase->setCurrentIndex(1);
            break;

        case 16:
            ui->cbBase->setCurrentIndex(0);
            break;

        default:
            break;
    }
}

int SpinBoxBaseSelector::base()
{
    return ui->sbValue->base();
}

void SpinBoxBaseSelector::setValue(qint64 val)
{
    ui->sbValue->setValue(val);
}

qint64 SpinBoxBaseSelector::value()
{
    return ui->sbValue->value();
}

void SpinBoxBaseSelector::clear()
{
    ui->sbValue->clear();
}

SpinBoxBaseSelector::~SpinBoxBaseSelector()
{
    delete ui;
}

void SpinBoxBaseSelector::on_sbValue_valueChanged(int value)
{
    emit valueChanged(static_cast<quint64>(value));
}

void SpinBoxBaseSelector::on_cbBase_currentIndexChanged(int index)
{
    int base = ui->cbBase->itemData(index).toInt();
    ui->sbValue->setBase(base);

    emit baseChanged(base);
}
