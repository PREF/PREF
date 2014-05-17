#include "dotplotviewmode.h"

DotPlotViewMode::DotPlotViewMode(QHexEditData *hexeditdata, QObject *parent): AbstractViewMode(hexeditdata, parent)
{
}

qint64 DotPlotViewMode::length() const
{
    return this->_hexeditdata->length();
}

qint64 DotPlotViewMode::offset(const QVector2D &v) const
{
    quint64 s = qMin(static_cast<qint64>(500), this->_hexeditdata->length());

    if(v.x() > s || v.y() > s)
        return -1;

    qint64 offset = this->_start + (v.x() + (v.y() / s));

    if(offset >= 0 && (offset <= this->_end))
        return offset;

    return -1;
}

void DotPlotViewMode::render(QPainter &painter, qint64 start, qint64 end, qint64 width)
{
    AbstractViewMode::render(painter, start, end, width);

    qint64 s = qMin(static_cast<qint64>(500), this->_hexeditdata->length());
    QHexEditDataReader reader(this->_hexeditdata);

    QImage img(s, s, QImage::Format_RGB888);
    img.fill(QColor(Qt::black));

    for(qint64 i = 0; i < s; i++)
    {
        if((start + i) >= this->_hexeditdata->length())
            break;

        uchar ib = reader.at(start + i);

        for(qint64 j = 0; j < s; j++)
        {
            if((start + j) >= this->_hexeditdata->length())
                break;

            uchar jb = reader.at(start + j);

            if(jb == ib)
                img.setPixel(j, i, qRgb(0, static_cast<uchar>((static_cast<qreal>(jb) * 0.75) + 64), 0));
        }
    }

    painter.drawImage(0, 0, img);
}
