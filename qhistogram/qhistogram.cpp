#include "qhistogram.h"

const int QHistogram::BAR_COUNT = 255;

QHistogram::QHistogram(QWidget *parent): QWidget(parent)
{
    this->_margin = 30;
    this->_originX = this->_margin;
    this->_endaxisY = this->_margin;
}

void QHistogram::setData(const ByteElaborator::CountResult& cr)
{
    this->_countresult = cr;
    this->update();
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

    if(!this->_countresult.Counts.empty() && this->_countresult.MaxCount)  /* Draw Y Axis Labels */
    {
        QString smaxval = QString::number(this->_countresult.MaxCount);
        QString smidval = QString::number(this->_countresult.MaxCount / 2);

        p.drawText(QPointF(this->_originX - fm.width(smaxval), this->_endaxisY + fm.height() / 2), smaxval);
        p.drawText(QPointF(this->_originX - fm.width(smidval), (this->_originY - this->_endaxisY) / 2  + fm.height() / 2), smidval);
    }
}

void QHistogram::drawBars(QPainter &p)
{
    qreal xpos = this->_originX;
    ByteElaborator::CountList& cl = this->_countresult.Counts;

    if(this->_barwidth < 2.0)
        xpos++;

    for(int i = 0; i <= QHistogram::BAR_COUNT; i++)
    {
        uintmax_t val = cl.at(i);

        if(val)
        {
            qreal barheight = (static_cast<qreal>(val) / static_cast<qreal>(this->_countresult.MaxCount) * this->_barheight);
            p.fillRect(QRectF(xpos, this->_originY - barheight, this->_barwidth, barheight), ByteColors::info(i).Color);

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

    if(!this->_countresult.Counts.empty())
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
