#include "structuremenu.h"

StructureMenu::StructureMenu(QWidget *parent): QMenu(parent)
{
    this->setTitle("Structure");

    this->_gotomenu = this->addMenu("Goto");
    this->_actgotostart = this->_gotomenu->addAction("Start Offset");
    this->_actgotoend = this->_gotomenu->addAction("End Offset");
    this->_actexport = this->addAction("Export");
    this->_actimport = this->addAction("Import");

    connect(this->_actgotostart, SIGNAL(triggered()), this, SIGNAL(gotoStartAction()));
    connect(this->_actgotoend, SIGNAL(triggered()), this, SIGNAL(gotoEndAction()));
    connect(this->_actexport, SIGNAL(triggered()), this, SIGNAL(exportAction()));
    connect(this->_actimport, SIGNAL(triggered()), this, SIGNAL(importAction()));
}

void StructureMenu::setGotoVisible(bool b)
{
    this->_gotomenu->setVisible(b);
}
