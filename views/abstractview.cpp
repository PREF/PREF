#include "abstractview.h"

AbstractView::AbstractView(const QString &viewname, QLabel* labelinfo, QWidget *parent): QWidget(parent), _lblinfo(labelinfo), _viewname(viewname)
{

}

AbstractView::~AbstractView()
{

}

const QString &AbstractView::viewName() const
{
    return this->_viewname;
}

void AbstractView::updateInfoText(const QString &s)
{
    this->_lblinfo->setText(s);
}
