#include "pixelviewmode.h"

PixelViewMode::PixelViewMode(QHexEdit *hexedit, QObject *parent): AbstractViewMode(hexedit, parent)
{
}

qint64 PixelViewMode::length() const
{
    return this->_hexedit->data()->length() - this->_hexedit->visibleStartOffset();
}

qint64 PixelViewMode::offset(const QVector2D &v) const
{
    qint64 offset = this->_hexedit->visibleStartOffset() + (v.x() + (v.y() * this->_width));

    if((offset >= 0) && (offset <= this->_hexedit->data()->length()))
        return offset;

    return -1;
}

void PixelViewMode::render(QPainter& painter, qint64 width)
{
    AbstractViewMode::render(painter, width);

    QHexEditData* hexeditdata = this->_hexedit->data();
    QHexEditDataReader reader(hexeditdata);
    qint64 s = this->_hexedit->visibleStartOffset(), h = this->preferredHeight(painter), e = qMin(hexeditdata->length(), this->_width * h);

    if(s > e)
        e = hexeditdata->length();

    this->_bits = reader.read(s, e - s);

    QImage img(reinterpret_cast<const uchar*>(this->_bits.constData()), this->_width, h, this->_width, QImage::Format_Indexed8);
    img.setColorTable(ByteColors::byteClassColorTable());
    painter.drawImage(0, 0, img);
}
