#include "abstractview.h"

AbstractView::AbstractView(QHexEditData* hexeditdata, QLabel *lblstatus, const QString &loadedfile, QWidget *parent): QWidget(parent), _hexeditdata(hexeditdata), _lblstatus(lblstatus), _loadedfile(loadedfile)
{
    hexeditdata->setParent(this); /* Take Ownership */
}

AbstractView::~AbstractView()
{

}

const QString &AbstractView::loadedFile() const
{
    return this->_loadedfile;
}

void AbstractView::updateToolBar(QToolBar* toolbar) const
{
    Q_UNUSED(toolbar);
}
