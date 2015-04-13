#ifndef CHARTWORKER_H
#define CHARTWORKER_H

#include <QtCore>
#include <chart/histogramchart.h>
#include <chart/entropychart.h>
#include "qhexedit/qhexeditdata.h"
#include "views/hexview/worker.h"
#include "prefsdk/qdatabuffer.h"

using namespace PrefLib;
using namespace PrefLib::Chart;

class ChartWorker: public Worker
{
    Q_OBJECT

    public:
        explicit ChartWorker(QObject *parent = 0);
        ~ChartWorker();
        void setData(HistogramChart *histogramchart, EntropyChart *entropychart, QHexEditData *hexeditdata);

    private:
        quint64 calculateBlockSize();

    protected:
        virtual void run();

    signals:
        void histogramChartCompleted(quint64 size);
        void entropyChartCompleted(quint64 size);

    private:
        HistogramChart* _histogramchart;
        EntropyChart* _entropychart;
        QHexEditData* _hexeditdata;
};

#endif // CHARTWORKER_H
