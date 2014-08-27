#include "qluadialog.h"

QLuaDialog::QLuaDialog(QWidget *parent): QDialog(parent)
{
}

void QLuaDialog::setPosition(int x, int y)
{
    this->move(x, y);
}

void QLuaDialog::setSize(int w, int h)
{
    this->resize(w, h);
}
