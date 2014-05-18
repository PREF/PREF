#include "abstractviewmode.h"

AbstractViewMode::AbstractViewMode(QHexEditData *hexeditdata, QObject *parent): QObject(parent), _hexeditdata(hexeditdata), _start(0), _end(this->_hexeditdata->length()), _width(256)
{

}

void AbstractViewMode::render(QPainter&, qint64 start, qint64 end, qint64 width)
{
    this->_start = start;
    this->_end = end;
    this->_width = width;
}

qint64 AbstractViewMode::preferredHeight(const QPainter &painter)
{
    QPaintDevice* paintdevice = painter.device();
    return qMin((this->_end - this->_start) / this->_width, static_cast<qint64>(paintdevice->height()));
}
