#include "chartworker.h"

ChartWorker::ChartWorker(QObject *parent): Worker(parent), _histogramchart(nullptr), _entropychart(nullptr), _databuffer(nullptr)
{

}

ChartWorker::~ChartWorker()
{

}

void ChartWorker::setData(HistogramChart* histogramchart, EntropyChart* entropychart, IO::DataBuffer *databuffer)
{
    //NOTE: Move To Thread?
    this->_histogramchart = histogramchart;
    this->_entropychart = entropychart;
    this->_databuffer = databuffer;
}

void ChartWorker::run()
{
    if(!this->_histogramchart || !this->_entropychart || !this->_databuffer)
        return;

    // NOTE: PrefLib Issue: do not count bytes twice
    dynamic_cast<AbstractChart*>(this->_histogramchart)->elaborate(this->_databuffer); //NOTE: Manage _cancontinue
    dynamic_cast<AbstractChart*>(this->_entropychart)->elaborate(this->_databuffer);  //NOTE: Manage _cancontinue

    emit dataEntropyCompleted();
}
