#include "dotplotviewmode.h"

DotPlotViewMode::DotPlotViewMode(QHexEdit *hexedit, QObject *parent): AbstractViewMode(hexedit, parent)
{
}

qint64 DotPlotViewMode::length() const
{
    return this->_hexedit->data()->length();
}

qint64 DotPlotViewMode::offset(const QVector2D &v) const
{
    qint64 l = this->_hexedit->data()->length();
    quint64 s = qMin(static_cast<qint64>(500), l);

    if(v.x() > s || v.y() > s)
        return -1;

    qint64 offset = this->_hexedit->visibleStartOffset() + (v.x() + (v.y() / s));

    if(offset >= 0 && (offset <= l))
        return offset;

    return -1;
}

void DotPlotViewMode::render(QPainter &painter, qint64 width)
{
    AbstractViewMode::render(painter, width);

    QHexEditData* hexeditdata = this->_hexedit->data();
    qint64 s = qMin(static_cast<qint64>(500), hexeditdata->length()), start = this->_hexedit->visibleStartOffset();
    QHexEditDataReader reader(hexeditdata);

    QImage img(s, s, QImage::Format_RGB888);
    img.fill(QColor(Qt::black));

    for(qint64 i = 0; i < s; i++)
    {
        if((start + i) >= hexeditdata->length())
            break;

        uchar ib = reader.at(start + i);

        for(qint64 j = 0; j < s; j++)
        {
            qint64 offset = start + j;

            if(offset >= hexeditdata->length())
                break;

            uchar jb = reader.at(offset);

            if(jb == ib)
            {
                uchar b = static_cast<uchar>((static_cast<qreal>(jb) * 0.75) + 64);

                if((offset >= this->_hexedit->selectionStart()) && (offset <= this->_hexedit->selectionEnd()))
                    img.setPixel(j, i, qRgb(b, b, 0));
                else
                    img.setPixel(j, i, qRgb(0, b, 0));
            }
        }
    }

    painter.drawImage(0, 0, img);
}
