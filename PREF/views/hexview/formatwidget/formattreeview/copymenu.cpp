#include "copymenu.h"

CopyMenu::CopyMenu(QWidget *parent) : QMenu(parent)
{
    this->setTitle("Copy");

    this->_copyoffset = this->addAction("Offset");
    this->_copyname = this->addAction("Name");
    this->_copyvalue = this->addAction("Value");

    connect(this->_copyoffset, SIGNAL(triggered()), this, SIGNAL(copyOffset()));
    connect(this->_copyname, SIGNAL(triggered()), this, SIGNAL(copyName()));
    connect(this->_copyvalue, SIGNAL(triggered()), this, SIGNAL(copyValue()));
}

void CopyMenu::setCopyValueVisible(bool b)
{
    this->_copyvalue->setVisible(b);
}
