#include "chartworker.h"
#include "loadeddata.h"

ChartWorker::ChartWorker(HistogramChart *histogramchart, EntropyChart *entropychart, QHexEditData *hexeditdata, QObject *parent): QThread(parent), _cancontinue(true)
{
    this->_histogramchart = histogramchart;
    this->_entropychart = entropychart;
    this->_hexeditdata = hexeditdata;
}

void ChartWorker::abort()
{
    this->_cancontinue = false;
}

void ChartWorker::run()
{
    if(!this->_hexeditdata)
        return;

    this->_cancontinue = true;
    LoadedData loadeddata(this->_hexeditdata);

    if(this->_histogramchart)
    {
        static_cast<AbstractChart*>(this->_histogramchart)->elaborate(&loadeddata, this->_cancontinue);
        emit histogramChartCompleted();
    }

    if(this->_entropychart)
    {
        static_cast<AbstractChart*>(this->_entropychart)->elaborate(&loadeddata, this->_cancontinue);
        emit entropyChartCompleted();
    }

    emit entropyCalculated(Algorithm::entropy(this->_histogramchart->result(), this->_hexeditdata->length()), this->_hexeditdata->length());
}
