#include "datatypesmenu.h"

DataTypesMenu::DataTypesMenu(QWidget *parent): QMenu(parent)
{
    this->_endiangroup = new QActionGroup(this);
    this->_endiangroup->setExclusive(true);

    this->_endianmenu = this->addMenu("Endian");

    this->_actbigendian = this->_endianmenu->addAction("Big Endian");
    this->_actbigendian->setActionGroup(this->_endiangroup);
    this->_actbigendian->setCheckable(true);

    this->_actlittleendian = this->_endianmenu->addAction("Little Endian");
    this->_actlittleendian->setActionGroup(this->_endiangroup);
    this->_actlittleendian->setCheckable(true);

    this->_numbasemenu = new NumericBaseMenu();
    this->addMenu(this->_numbasemenu);

    connect(this->_actlittleendian, SIGNAL(triggered()), this, SLOT(onLittleEndianTriggered()));
    connect(this->_actbigendian, SIGNAL(triggered()), this, SLOT(onBigEndianTriggered()));
    connect(this->_numbasemenu, SIGNAL(baseChanged(int)), this, SIGNAL(baseChanged(int)));
}

void DataTypesMenu::setBase(int b)
{
    this->_numbasemenu->setBase(b);
    emit baseChanged(b);
}

void DataTypesMenu::setEndian(Endianness::Type endian)
{
    if(endian == Endianness::LittleEndian)
        this->_actlittleendian->setChecked(true);
    else
        this->_actbigendian->setChecked(true);

    emit endianChanged(endian);
}

void DataTypesMenu::onLittleEndianTriggered()
{
    emit endianChanged(Endianness::LittleEndian);
}

void DataTypesMenu::onBigEndianTriggered()
{
    emit endianChanged(Endianness::BigEndian);
}
