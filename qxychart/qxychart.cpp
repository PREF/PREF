#include "qxychart.h"

const qint64 QXYChart::MARGIN = 30;

QXYChart::QXYChart(QWidget *parent): QWidget(parent), _xbase(10), _ybase(10), _xmin(0), _ymin(0), _xmax(100), _ymax(100)
{
    this->_originX = QXYChart::MARGIN;
    this->_endaxisY = QXYChart::MARGIN;
}

void QXYChart::setXBase(int base)
{
    this->_xbase = base;

    this->update();
}

void QXYChart::setYBase(int base)
{
    this->_ybase = base;

    this->update();
}

void QXYChart::setXRange(qreal min, qreal max)
{
    this->_xmin = min;
    this->_xmax = max;

    this->update();
}

void QXYChart::setYRange(qreal min, qreal max)
{
    this->_ymin = min;
    this->_ymax = max;

    this->update();
}

void QXYChart::setPoints(QList<QPointF> &points)
{
    this->_points = points;

    std::sort(this->_points.begin(), this->_points.end(), &QXYChart::sortPoints);
    this->update();
}

bool QXYChart::sortPoints(const QPointF &p1, const QPointF &p2)
{
    return p1.x() < p2.x();
}

void QXYChart::drawAxis(QPainter &p)
{
    QFontMetrics fm = this->fontMetrics();
    int charwidth = fm.width(" ");
    QString xminstring = QString::number(static_cast<qint64>(this->_xmin), this->_xbase).toUpper();
    QString xmidstring = QString::number(static_cast<qint64>((this->_xmax + this->_xmin) / 2), this->_xbase).toUpper();
    QString xmaxstring = QString::number(static_cast<qint64>(this->_xmax), this->_xbase).toUpper();
    QString yminstring = QString::number(static_cast<qint64>(this->_ymin), this->_ybase).toUpper();
    QString ymidstring = QString::number(static_cast<qint64>((this->_ymax + this->_ymin) / 2), this->_ybase).toUpper();
    QString ymaxstring = QString::number(static_cast<qint64>(this->_ymax), this->_ybase).toUpper();

    p.drawLine(QPointF(this->_originX, this->_originY), QPointF(this->_endaxisX, this->_originY)); // X Axis
    p.drawLine(QPointF(this->_originX, this->_originY), QPointF(this->_originX, this->_endaxisY)); // Y Axis

    /* Draw X Axis Labels */
    p.drawText(QPointF(this->_originX - (fm.width(xminstring) / 2), this->_originY + fm.height()), xminstring);
    p.drawText(QPointF(((this->_endaxisX + this->_originX) / 2) - (fm.width(xmidstring) / 2), this->_originY + fm.height()), xmidstring);
    p.drawText(QPointF(this->_endaxisX - (fm.width(xmaxstring) / 2), this->_originY + fm.height()), xmaxstring);

    /* Draw Y Axis Labels */
    p.drawText(QPointF(this->_originX - fm.width(yminstring) - charwidth, this->_originY), yminstring);
    p.drawText(QPointF(this->_originX - fm.width(ymaxstring) - charwidth, ((this->_originY + this->_endaxisY) / 2) + (fm.height() / 2)), ymidstring);
    p.drawText(QPointF(this->_originX - fm.width(ymaxstring) - charwidth, QXYChart::MARGIN), ymaxstring);
}

void QXYChart::drawPoints(QPainter &p)
{
    QPointF lastptc;

    for(int i = 0; i < this->_points.length(); i++)
    {        
        const QPointF& pt = this->_points[i];
        QPointF ptc = this->convertPoint(pt);

        if(!i)
        {
            lastptc = ptc;
            continue;
        }

        p.setPen(ByteColors::entropyColor(pt.y()));
        p.drawLine(lastptc, ptc);
        lastptc = ptc;
    }
}

QPointF QXYChart::convertPoint(const QPointF &p)
{
    qreal xrange = this->_xmax - this->_xmin;
    qreal yrange = this->_ymax - this->_ymin;
    qreal scaledx = this->_originX + (((p.x() - this->_xmin) / xrange) * (this->_endaxisX - this->_originX));
    qreal scaledy = this->_originY - (((p.y() - this->_ymin) / yrange) * (this->_originY - this->_endaxisY));

    return QPointF(scaledx, scaledy);
}

void QXYChart::resizeEvent(QResizeEvent* e)
{
    this->_originY = this->height() - QXYChart::MARGIN;
    this->_endaxisX = this->width() - QXYChart::MARGIN;

    QWidget::resizeEvent(e);
}

void QXYChart::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setBackgroundMode(Qt::TransparentMode);

    this->drawAxis(p);

    if(!this->_points.isEmpty())
        this->drawPoints(p);
}
