#ifndef QXYCHART_H
#define QXYCHART_H

#include <QWidget>
#include <QPainter>
#include <chart/entropychart.h>
#include <support/bytecolors.h>

using namespace PrefLib::Chart;
using namespace PrefLib::Support;

class QXYChart : public QWidget
{
    Q_OBJECT

    public:
        explicit QXYChart(QWidget *parent = 0);
        ~QXYChart();
        void setXBase(int base);
        void setYBase(int base);
        void setXRange(qreal min, qreal max);
        void setYRange(qreal min, qreal max);
        EntropyChart* chart() const;

    private:
        void interpolate(QPainterPath &path, const QPointF& start, const QPointF &end);
        void drawAxis(QPainter &p);
        void drawPoints(QPainter& p);
        QPointF convertPoint(const EntropyChart::EntropyPoint &p);

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
        EntropyChart* _entroypchart;

    private:
        static const qint64 MARGIN;
};

#endif // QXYCHART_H
