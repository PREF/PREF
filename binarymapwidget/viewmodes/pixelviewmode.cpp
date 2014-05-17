#include "pixelviewmode.h"

PixelViewMode::PixelViewMode(QHexEditData *hexeditdata, QObject *parent): AbstractViewMode(hexeditdata, parent)
{
}

qint64 PixelViewMode::length() const
{
    return this->_end - this->_start;
}

qint64 PixelViewMode::offset(const QVector2D &v) const
{
    qint64 offset = this->_start + (v.x() + (v.y() * this->_width));

    if((offset >= 0) && (offset <= this->_end))
        return offset;

    return -1;
}

void PixelViewMode::render(QPainter& painter, qint64 start, qint64 end, qint64 width)
{
    AbstractViewMode::render(painter, start, end, width);

    QHexEditDataReader reader(this->_hexeditdata);
    qint64 h = this->preferredHeight(painter);
    qint64 l = qMin(this->_hexeditdata->length(), this->_width * h);
    this->_bits = reader.read(this->_start, l);

    QImage img(reinterpret_cast<const uchar*>(this->_bits.constData()), this->_width, h, this->_width, QImage::Format_Indexed8);
    img.setColorTable(ByteColors::byteClassColorTable());
    painter.drawImage(0, 0, img);
}
