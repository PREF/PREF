#include "abstractview.h"

AbstractView::AbstractView(QLabel* labelinfo, QWidget *parent): QWidget(parent), _lblinfo(labelinfo)
{

}

AbstractView::~AbstractView()
{

}

void AbstractView::updateInfoText(const QString &s)
{
    this->_lblinfo->setText(s);
}
