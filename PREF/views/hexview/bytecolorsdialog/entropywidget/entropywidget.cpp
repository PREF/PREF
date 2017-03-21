#include "entropywidget.h"

EntropyWidget::EntropyWidget(QWidget *parent): QWidget(parent)
{
    this->adjust();
}

void EntropyWidget::adjust()
{
    qreal sz = static_cast<qreal>(qMax(this->width(), this->height()));

    this->_step = 1.0 / sz;
    this->_stepwidth = sz * this->_step;
}

void EntropyWidget::paintEvent(QPaintEvent*)
{
    qreal p = 0.0;
    bool ishorz = this->width() > this->height();
    QPainter painter(this);

    for(qreal e = 0.0; e <= 1.0; e += this->_step, p += this->_stepwidth)
    {
        if(ishorz)
            painter.fillRect(QRectF(p, 0, this->_stepwidth, this->height()), QBrush(ByteColors::entropyColor(e)));
        else
            painter.fillRect(QRectF(0, p, this->width(), this->_stepwidth), QBrush(ByteColors::entropyColor(e)));
    }

    painter.setPen(QColor(Qt::black));
    painter.drawRect(0, 0, this->width() - 1, this->height() - 1);
}

void EntropyWidget::resizeEvent(QResizeEvent*)
{
    this->adjust();
}
