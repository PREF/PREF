#include "chartcontainer.h"

ChartContainer::ChartContainer(QWidget *parent): QWidget(parent)
{
    this->_vboxlayout = new QVBoxLayout(this);
    this->_vboxlayout->setContentsMargins(0, 0, 0, 0);

    this->_histogram = new QHistogram(this);
    this->_xychart = new QXYChart(this);
    this->_xychart->setVisible(false);

    this->_vboxlayout->addWidget(this->_histogram);
    this->_vboxlayout->addWidget(this->_xychart);
    this->setLayout(this->_vboxlayout);
}

QHistogram *ChartContainer::histogram()
{
    return this->_histogram;
}

QXYChart *ChartContainer::xyChart()
{
    return this->_xychart;
}

void ChartContainer::switchChart()
{
    this->_histogram->setVisible(!this->_histogram->isVisible());
    this->_xychart->setVisible(!this->_xychart->isVisible());
}
