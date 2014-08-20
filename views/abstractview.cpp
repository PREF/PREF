#include "abstractview.h"

AbstractView::AbstractView(QHexEditData* hexeditdata, const QString &viewname, QLabel* labelinfo, AbstractView::LoadedViews& loadedviews, QWidget *parent): QWidget(parent), _lblinfo(labelinfo), _viewname(viewname), _hexeditdata(hexeditdata) ,_loadedviews(loadedviews)
{
    if(hexeditdata)
        loadedviews[hexeditdata] = this;
}

AbstractView::~AbstractView()
{
    if(this->_hexeditdata)
        this->_loadedviews.remove(this->_hexeditdata);
}

const QString &AbstractView::viewName() const
{
    return this->_viewname;
}

void AbstractView::updateInfoText(const QString &s)
{
    this->_lblinfo->setText(s);
}
