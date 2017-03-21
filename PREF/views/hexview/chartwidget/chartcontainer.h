#ifndef CHARTCONTAINER_H
#define CHARTCONTAINER_H

#include <QWidget>
#include "qhistogram/qhistogram.h"
#include "qxychart/qxychart.h"

class ChartContainer : public QWidget
{
    Q_OBJECT

    public:
        explicit ChartContainer(QWidget *parent = 0);
        QHistogram* histogram();
        QXYChart* xyChart();

    public slots:
        void switchChart();

    private:
        QVBoxLayout* _vboxlayout;
        QHistogram* _histogram;
        QXYChart* _xychart;
};

#endif // CHARTCONTAINER_H
