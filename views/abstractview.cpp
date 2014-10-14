#include "abstractview.h"

AbstractView::AbstractView(QHexEditData* hexeditdata, const QString &viewname, QLabel* labelinfo, QWidget *parent): QWidget(parent), _lblinfo(labelinfo), _hexeditdata(hexeditdata), _viewname(viewname)
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
