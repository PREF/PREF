#ifndef CHARTWORKER_H
#define CHARTWORKER_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "views/hexview/worker.h"
#include "prefsdk/qdatabuffer.h"
#include <chart/histogramchart.h>
#include <chart/entropychart.h>

using namespace PrefLib;
using namespace PrefLib::Chart;

class ChartWorker : public Worker
{
    Q_OBJECT

    public:
        explicit ChartWorker(QObject *parent = 0);
        ~ChartWorker();
        void setData(HistogramChart *histogramchart, EntropyChart *entropychart, IO::DataBuffer *databuffer);

    private:
        quint64 calculateBlockSize();

    protected:
        virtual void run();

    signals:
        void occurrencesListCompleted();
        void dataEntropyCompleted();

    private:
        HistogramChart* _histogramchart;
        EntropyChart* _entropychart;
        IO::DataBuffer* _databuffer;
};

#endif // CHARTWORKER_H
