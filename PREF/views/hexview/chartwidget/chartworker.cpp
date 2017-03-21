#include "chartworker.h"

ChartWorker::ChartWorker(QObject *parent): Worker(parent), _histogramchart(nullptr), _entropychart(nullptr), _hexeditdata(nullptr)
{

}

ChartWorker::~ChartWorker()
{

}

void ChartWorker::setData(HistogramChart* histogramchart, EntropyChart* entropychart, QHexEditData *hexeditdata)
{
    this->_histogramchart = histogramchart;
    this->_entropychart = entropychart;
    this->_hexeditdata = hexeditdata;
}

void ChartWorker::run()
{
    if(!this->_hexeditdata)
        return;

    QDataBuffer databuffer(this->_hexeditdata);

    if(this->_histogramchart)
    {
        dynamic_cast<AbstractChart*>(this->_histogramchart)->elaborate(&databuffer, this->_cancontinue);
        emit histogramChartCompleted(this->_hexeditdata->length());
    }

    if(this->_entropychart)
    {
        dynamic_cast<AbstractChart*>(this->_entropychart)->elaborate(&databuffer, this->_cancontinue);
        emit entropyChartCompleted(this->_hexeditdata->length());
    }
}
