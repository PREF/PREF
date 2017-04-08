#include "dotplotviewmode.h"
#include <qhexedit/qhexedit.h>
#include <QPainter>

DotPlotViewMode::DotPlotViewMode(QHexEdit *hexedit, QObject *parent): AbstractViewMode(hexedit, parent)
{
}

integer_t DotPlotViewMode::size() const
{
    return this->_hexedit->document()->length();
}

integer_t DotPlotViewMode::offset(const QPoint &p) const
{
    integer_t l = this->_hexedit->document()->length();
    sinteger_t s = qMin(sinteger_t(500), static_cast<sinteger_t>(l));

    if(p.x() > s || p.y() > s)
        return -1;

    QHexMetrics* metrics = this->_hexedit->metrics();
    sinteger_t offset = metrics->visibleStartOffset() + (p.x() + (p.y() / s));

    if(offset >= 0 && (offset <= l))
        return offset;

    return -1;
}

void DotPlotViewMode::render(QPainter *painter, qint64 width)
{
    AbstractViewMode::render(painter, width);

    QHexMetrics* metrics = this->_hexedit->metrics();
    QHexDocument* document = this->_hexedit->document();
    QHexCursor* cursor = document->cursor();
    integer_t s = std::min(integer_t(500), document->length()), start = metrics->visibleStartOffset();

    QImage img(s, s, QImage::Format_RGB888);
    img.fill(QColor(Qt::black));

    for(integer_t i = 0; i < s; i++)
    {
        if((start + i) >= document->length())
            break;

        uchar ib = document->at(start + i);

        for(integer_t j = 0; j < s; j++)
        {
            integer_t offset = start + j;

            if(offset >= document->length())
                break;

            uchar jb = document->at(offset);

            if(jb == ib)
            {
                uchar b = static_cast<uchar>((static_cast<qreal>(jb) * 0.75) + 64);

                if((offset >= cursor->selectionStart()) && (offset <= cursor->selectionEnd()))
                    img.setPixel(j, i, qRgb(b, b, 0));
                else
                    img.setPixel(j, i, qRgb(0, b, 0));
            }
        }
    }

    painter->drawImage(0, 0, img);
}
