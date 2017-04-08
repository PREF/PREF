#include "abstractview.h"

AbstractView::AbstractView(QHexDocument* document, QLabel *lblstatus, const QString &loadedfile, QWidget *parent): QWidget(parent), _document(document), _lblstatus(lblstatus), _loadedfile(loadedfile)
{
    document->setParent(this); // NOTE: Take Ownership
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
