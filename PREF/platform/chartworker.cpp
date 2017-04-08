#include "chartworker.h"
#include "loadeddata.h"

ChartWorker::ChartWorker(HistogramChart *histogramchart, EntropyChart *entropychart, QHexDocument *document, QObject *parent): BasicWorker(document, parent)
{
    this->_histogramchart = histogramchart;
    this->_entropychart = entropychart;
    this->_document = document;
}

void ChartWorker::run()
{
    if(!this->_document)
        return;

    this->_cancontinue = true;
    LoadedData loadeddata(this->_document);

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

    emit entropyCalculated(Algorithm::entropy(this->_histogramchart->result(), this->_document->length()), this->_document->length());
}
