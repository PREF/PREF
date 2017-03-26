#ifndef CHARTWORKER_H
#define CHARTWORKER_H

#include <chart/histogramchart.h>
#include <chart/entropychart.h>
#include "basicworker.h"

using namespace PrefLib::Chart;

class ChartWorker : public BasicWorker
{
    Q_OBJECT

    public:
        explicit ChartWorker(HistogramChart* histogramchart, EntropyChart* entropychart, QHexEditData *hexeditdata, QObject *parent = 0);

    protected:
        virtual void run();

    signals:
        void histogramChartCompleted();
        void entropyChartCompleted();
        void entropyCalculated(double e, quint64 size);

    private:
        HistogramChart* _histogramchart;
        EntropyChart* _entropychart;
};

#endif // CHARTWORKER_H
