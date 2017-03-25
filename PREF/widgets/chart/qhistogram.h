#ifndef QHISTOGRAM_H
#define QHISTOGRAM_H

#include <QWidget>
#include <chart/histogramchart.h>

using namespace PrefLib::Chart;

class QHistogram : public QWidget
{
    Q_OBJECT

    public:
        explicit QHistogram(QWidget *parent = 0);
        ~QHistogram();
        HistogramChart* chart() const;

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
        HistogramChart* _histogramchart;

    private: /* Constants */
        static const int BAR_COUNT;
};

#endif // QHISTOGRAM_H
