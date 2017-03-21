#include "numericbasemenu.h"

NumericBaseMenu::NumericBaseMenu(QWidget *parent): QMenu(parent)
{
    this->setTitle("Base");

    this->_actgroup = new QActionGroup(this);
    this->_actgroup->setExclusive(true);

    this->_actbinary = this->addAction("Binary");
    this->_actoctal = this->addAction("Octal");
    this->_actdecimal = this->addAction("Decimal");
    this->_acthexadecimal = this->addAction("Hexadecimal");

    this->_actbinary->setActionGroup(this->_actgroup);
    this->_actbinary->setCheckable(true);
    this->_actoctal->setCheckable(true);
    this->_actoctal->setActionGroup(this->_actgroup);
    this->_actdecimal->setCheckable(true);
    this->_actdecimal->setActionGroup(this->_actgroup);
    this->_acthexadecimal->setCheckable(true);
    this->_acthexadecimal->setActionGroup(this->_actgroup);

    this->_acthexadecimal->setChecked(true);

    /* Forward Actions' Signals */
    connect(this->_actbinary, SIGNAL(triggered()), this, SLOT(onActionBinaryTriggered()));
    connect(this->_actoctal, SIGNAL(triggered()), this, SLOT(onActionOctalTriggered()));
    connect(this->_actdecimal, SIGNAL(triggered()), this, SLOT(onActionDecimalTriggered()));
    connect(this->_acthexadecimal, SIGNAL(triggered()), this, SLOT(onActionHexadecimalTriggered()));
}

void NumericBaseMenu::setBase(int base)
{
    switch(base)
    {
        case 2:
            this->_actbinary->setChecked(true);
            break;

        case 8:
            this->_actoctal->setChecked(true);
            break;

        case 10:
            this->_actdecimal->setChecked(true);
            break;

        case 16:
            this->_acthexadecimal->setChecked(true);
            break;

        default:
            break;
    }
}

void NumericBaseMenu::onActionBinaryTriggered()
{
    emit baseChanged(2);
}

void NumericBaseMenu::onActionOctalTriggered()
{
    emit baseChanged(8);
}

void NumericBaseMenu::onActionDecimalTriggered()
{
    emit baseChanged(10);
}

void NumericBaseMenu::onActionHexadecimalTriggered()
{
    emit baseChanged(16);
}
