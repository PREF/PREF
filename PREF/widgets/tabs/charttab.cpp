#include "charttab.h"
#include "ui_charttab.h"
#include "../../platform/chartworker.h"
#include <support/bytecolors.h>

using namespace PrefLib::Support;

ChartTab::ChartTab(QWidget *parent) : QWidget(parent), ui(new Ui::ChartTab)
{
    ui->setupUi(this);
    ui->tbSwitchChart->setIcon(QIcon(":/res/xychart.png"));
}

void ChartTab::initialize(QHexEditData *hexeditdata)
{
    ChartWorker* chartworker = new ChartWorker(ui->chartContainer->histogram()->chart(), ui->chartContainer->xyChart()->chart(), hexeditdata, this);

    connect(chartworker, &ChartWorker::histogramChartCompleted, [this]() { ui->chartContainer->histogram()->update(); });
    connect(chartworker, &ChartWorker::entropyChartCompleted, [this]() { ui->chartContainer->xyChart()->update(); });
    connect(chartworker, &ChartWorker::entropyCalculated, this, &ChartTab::updateEntropy);
    connect(chartworker, &ChartWorker::finished, chartworker, &ChartWorker::deleteLater);

    chartworker->start();
}

ChartTab::~ChartTab()
{
    delete ui;
}

void ChartTab::updateEntropy(double e, quint64 size)
{
    ui->chartContainer->xyChart()->setXBase(16);
    ui->chartContainer->xyChart()->setXRange(0, size);
    ui->chartContainer->xyChart()->setYRange(0, 1);

    ui->lblEntropy->setText(QString::number(e));

    QPalette p = ui->lblEntropy->palette();
    p.setColor(ui->lblEntropy->foregroundRole(), ByteColors::entropyColor(e));
    ui->lblEntropy->setPalette(p);
}

void ChartTab::on_tbSwitchChart_clicked()
{
    ui->chartContainer->switchChart();

    if(ui->chartContainer->histogram()->isVisible())
        ui->tbSwitchChart->setIcon(QIcon(":/res/xychart.png"));
    else
        ui->tbSwitchChart->setIcon(QIcon(":/res/histogram.png"));
}
