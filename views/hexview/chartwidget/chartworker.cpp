#include "chartworker.h"

ChartWorker::ChartWorker(QObject *parent): Worker(parent), _histogramchart(nullptr), _entropychart(nullptr), _hexeditdata(nullptr)
{

}

ChartWorker::~ChartWorker()
{

}

void ChartWorker::setData(HistogramChart* histogramchart, EntropyChart* entropychart, QHexEditData *hexeditdata)
{
    //NOTE: Move To Thread?
    this->_histogramchart = histogramchart;
    this->_entropychart = entropychart;
    this->_hexeditdata = hexeditdata;
}

void ChartWorker::run()
{
    if(!this->_histogramchart || !this->_entropychart || !this->_hexeditdata)
        return;

    // NOTE: do not count bytes twice (PrefLib Issue?)
    QDataBuffer databuffer(this->_hexeditdata);
    dynamic_cast<AbstractChart*>(this->_histogramchart)->elaborate(&databuffer); //NOTE: Manage _cancontinue
    dynamic_cast<AbstractChart*>(this->_entropychart)->elaborate(&databuffer);  //NOTE: Manage _cancontinue

    emit dataEntropyCompleted();
}
