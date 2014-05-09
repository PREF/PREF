#include "qhistogram.h"

const int QHistogram::BAR_COUNT = 255;

QHistogram::QHistogram(QWidget *parent): QWidget(parent)
{
    this->_margin = 30;
    this->_originX = this->_margin;
    this->_endaxisY = this->_margin;
}

void QHistogram::setData(const QList<qint64>& data)
{
    this->_data = data;
    this->update();
}

qint64 QHistogram::maxValue()
{
    qint64 max = 0;

    if(!this->_data.isEmpty())
    {
        for(int i = 0; i < QHistogram::BAR_COUNT; i++)
        {
            if(this->_data[i] > max)
                max = this->_data[i];
        }
    }

    return max;
}

void QHistogram::drawAxis(QPainter& p)
{
    static QString numstart = "0";
    static QString nummid = "128";
    static QString numend = "255";

    p.drawLine(QPointF(this->_originX, this->_originY), QPointF(this->_endaxisX, this->_originY)); // X Axis
    p.drawLine(QPointF(this->_originX, this->_originY), QPointF(this->_originX, this->_endaxisY)); // Y Axis

    QFontMetrics fm = this->fontMetrics();

    p.drawText(QPointF(this->_originX - fm.width(numstart), this->_originY + fm.height()), numstart);
    p.drawText(QPointF(((this->_endaxisX - this->_originX) / 2) + (fm.width(nummid) / 2), this->_originY + fm.height()), nummid);
    p.drawText(QPointF(this->_endaxisX - (fm.width(numend) / 2), this->_originY + fm.height()), numend);

    if(!this->_data.isEmpty())
    {
        quint64 maxvalue = this->maxValue();

        if(maxvalue) /* Draw Y Axis Labels */
        {
            QString smaxval = QString::number(maxvalue);
            QString smidval = QString::number(maxvalue / 2);

            p.drawText(QPointF(this->_originX - fm.width(smaxval), this->_endaxisY + fm.height() / 2), smaxval);
            p.drawText(QPointF(this->_originX - fm.width(smidval), (this->_originY - this->_endaxisY) / 2  + fm.height() / 2), smidval);
        }
    }
}

void QHistogram::drawBars(QPainter &p)
{
    qreal xpos = this->_originX;

    if(this->_barwidth < 2.0)
        xpos++;

    quint64 maxval = this->maxValue();

    for(int i = 0; i <= QHistogram::BAR_COUNT; i++)
    {
        quint64 val = this->_data[i];

        if(val)
        {
            qreal barheight = (static_cast<qreal>(val) / static_cast<qreal>(maxval) * this->_barheight);
            p.fillRect(QRectF(xpos, this->_originY - barheight, this->_barwidth, barheight), ByteColors::byteClassColor(i));

            if(this->_barwidth >= 2.0)
                p.drawRect(QRectF(xpos, this->_originY - barheight, this->_barwidth, barheight));
        }

        xpos += this->_barwidth;
    }
}

void QHistogram::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setBackgroundMode(Qt::TransparentMode);
    p.setPen(this->palette().color(QPalette::WindowText));

    this->drawAxis(p);

    if(!this->_data.isEmpty())
        this->drawBars(p);
}

void QHistogram::resizeEvent(QResizeEvent*)
{
    this->_originY = this->height() - this->_margin;
    this->_barheight = this->_originY - this->_margin;
    this->_barwidth = ((qreal)(this->width() - this->_margin) - this->_originX) / (qreal)QHistogram::BAR_COUNT;
    this->_endaxisX = this->_margin + (this->_barwidth * QHistogram::BAR_COUNT);

    if(this->_barwidth < 2.0)
        this->_endaxisX += 2;
}
