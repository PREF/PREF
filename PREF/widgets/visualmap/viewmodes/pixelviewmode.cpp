#include "pixelviewmode.h"
#include <qhexedit/qhexedit.h>
#include <support/bytecolors.h>
#include <QPainter>

using namespace PrefLib::Support;

PixelViewMode::PixelViewMode(QHexEdit *hexedit, QObject *parent): AbstractViewMode(hexedit, parent)
{
}

integer_t PixelViewMode::size() const
{
    QHexMetrics* metrics = this->_hexedit->metrics();
    return metrics->document()->length() - metrics->visibleStartOffset();
}

integer_t PixelViewMode::offset(const QPoint &p) const
{
    QHexMetrics* metrics = this->_hexedit->metrics();
    return metrics->visibleStartOffset() + (p.x() + (p.y() * this->_width));
}

void PixelViewMode::render(QPainter *painter, qint64 width)
{
    AbstractViewMode::render(painter, width);

    QHexMetrics* metrics = this->_hexedit->metrics();
    QHexDocument* document = this->_hexedit->document();
    qint64 s = metrics->visibleStartOffset(), h = this->preferredHeight(painter), e = std::min(document->length(), static_cast<integer_t>(this->_width * h));

    if(s > e)
        e = document->length();

    this->_bits = document->read(s, e - s);

    QImage img(reinterpret_cast<const uchar*>(this->_bits.constData()), this->_width, h, this->_width, QImage::Format_Indexed8);
    //FIXME: img.setColorTable(ByteColors::colorTable());
    painter->drawImage(0, 0, img);
}
