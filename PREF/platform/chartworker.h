#ifndef CHARTWORKER_H
#define CHARTWORKER_H

#include <QThread>
#include <qhexedit/qhexeditdata.h>
#include <chart/histogramchart.h>
#include <chart/entropychart.h>

using namespace PrefLib::Chart;

class ChartWorker : public QThread
{
    Q_OBJECT

    public:
        explicit ChartWorker(HistogramChart* histogramchart, EntropyChart* entropychart, QHexEditData *hexeditdata, QObject *parent = 0);
        void abort();

    protected:
        virtual void run();

    signals:
        void histogramChartCompleted();
        void entropyChartCompleted();
        void entropyCalculated(double e, quint64 size);

    private:
        HistogramChart* _histogramchart;
        EntropyChart* _entropychart;
        QHexEditData* _hexeditdata;
        bool _cancontinue;
};

#endif // CHARTWORKER_H
