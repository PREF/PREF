#ifndef QXYCHART_H
#define QXYCHART_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include "prefsdk/bytecolors.h"

using namespace PrefSDK;

class QXYChart : public QWidget
{
    Q_OBJECT

    public:
        explicit QXYChart(QWidget *parent = 0);
        void setXBase(int base);
        void setYBase(int base);
        void setXRange(qreal min, qreal max);
        void setYRange(qreal min, qreal max);
        void setPoints(QList<QPointF>& points);

    private:
        static bool sortPoints(const QPointF& p1, const QPointF& p2);

    private:
        void interpolate(QPainterPath &path, const QPointF& start, const QPointF &end);
        void drawAxis(QPainter &p);
        void drawPoints(QPainter& p);
        QPointF convertPoint(const QPointF &p);

    protected:
        virtual void resizeEvent(QResizeEvent* e);
        virtual void paintEvent(QPaintEvent*);

    private:
        int _xbase;
        int _ybase;
        qreal _xmin;
        qreal _ymin;
        qreal _xmax;
        qreal _ymax;
        qreal _originX;
        qreal _originY;
        qreal _endaxisX;
        qreal _endaxisY;
        QList<QPointF> _points;

    private:
        static const qint64 MARGIN;
};

#endif // QXYCHART_H
