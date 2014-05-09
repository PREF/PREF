#ifndef QHISTOGRAM_H
#define QHISTOGRAM_H

#include <QtCore>
#include <QtWidgets>
#include "prefsdk/bytecolors.h"

using namespace PrefSDK;

class QHistogram : public QWidget
{
    Q_OBJECT

    public:
        explicit QHistogram(QWidget *parent = 0);
        qint64 maxValue();

    public slots:
        void setData(const QList<qint64> &data);

    private:
        void drawAxis(QPainter &p);
        void drawBars(QPainter &p);

    protected:
        void paintEvent(QPaintEvent*);
        void resizeEvent(QResizeEvent*);

    private:
        qreal _originX;
        qreal _originY;
        qreal _endaxisX;
        qreal _endaxisY;
        qreal _barwidth;
        qreal _barheight;
        quint64 _margin;
        QList<qint64> _data;

    private: /* Constants */
        static const int BAR_COUNT;
};

#endif // QHISTOGRAM_H
