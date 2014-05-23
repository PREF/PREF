#include "abstractviewmode.h"

AbstractViewMode::AbstractViewMode(QHexEdit *hexedit, QObject *parent): QObject(parent), _hexedit(hexedit), _width(256)
{

}

void AbstractViewMode::render(QPainter&, qint64 width)
{
    this->_width = width;
}

qint64 AbstractViewMode::preferredHeight(const QPainter &painter)
{
    QPaintDevice* paintdevice = painter.device();
    return qMin((this->_hexedit->data()->length() - this->_hexedit->visibleStartOffset()) / this->_width, static_cast<qint64>(paintdevice->height()));
}
