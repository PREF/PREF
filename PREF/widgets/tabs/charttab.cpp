#include "charttab.h"
#include "ui_charttab.h"
#include <support/bytecolors.h>

using namespace PrefLib::Support;

ChartTab::ChartTab(QWidget *parent) : QWidget(parent), ui(new Ui::ChartTab)
{
    ui->setupUi(this);
    ui->tbSwitchChart->setIcon(QIcon(":/res/xychart.png"));
}

QHistogram *ChartTab::histogram() const
{
    return ui->chartContainer->histogram();
}

QXYChart *ChartTab::xyChart() const
{
    return ui->chartContainer->xyChart();
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
