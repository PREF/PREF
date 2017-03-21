#ifndef QHISTOGRAM_H
#define QHISTOGRAM_H

#include <QtCore>
#include <QtWidgets>
#include <chart/histogramchart.h>
#include <support/bytecolors.h>

using namespace PrefLib;
using namespace PrefLib::Support;

class QHistogram : public QWidget
{
    Q_OBJECT

    public:
        explicit QHistogram(QWidget *parent = 0);

    public slots:
        void setData(const ByteElaborator::CountResult& cr);

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
        ByteElaborator::CountResult _countresult;

    private: /* Constants */
        static const int BAR_COUNT;
};

#endif // QHISTOGRAM_H
