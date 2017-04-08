#include "abstractviewmode.h"
#include <QPainter>

AbstractViewMode::AbstractViewMode(QHexEdit *hexedit, QObject *parent): QObject(parent), _hexedit(hexedit), _width(256)
{

}

void AbstractViewMode::render(QPainter*, qint64 width)
{
    this->_width = width;
}

qint64 AbstractViewMode::preferredHeight(const QPainter *painter)
{
    QPaintDevice* paintdevice = painter->device();
    QHexMetrics* metrics = this->_hexedit->metrics();

    return qMin(static_cast<qint64>(metrics->document()->length() - metrics->visibleStartOffset()) / this->_width,
                static_cast<qint64>(paintdevice->height()));
}
