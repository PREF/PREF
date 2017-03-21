#include "chartwidget.h"
#include "ui_chartwidget.h"

ChartWidget::ChartWidget(QWidget *parent): WorkerTab(parent), ui(new Ui::ChartWidget), _histogrammodel(nullptr)
{
    this->_xycharticon = QIcon(":/misc_icons/res/xychart.png");
    this->_histogramicon = QIcon(":/misc_icons/res/histogram.png");

    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);
    ui->tbSwitchChart->setIcon(this->_xycharticon);

    connect(&this->_worker, SIGNAL(started()), this, SIGNAL(workStarted()));
    connect(&this->_worker, SIGNAL(histogramChartCompleted(quint64)), this, SLOT(onHistogramChartCompleted(quint64)));
    connect(&this->_worker, SIGNAL(entropyChartCompleted(quint64)), this, SLOT(onEntropyChartCompleted(quint64)));
    connect(&this->_worker, SIGNAL(finished()), this, SIGNAL(workFinished()));
}

void ChartWidget::plot(QHexEditData* hexeditdata)
{
    if(!hexeditdata || !hexeditdata->length())
        return;

    this->_histogrammodel = new HistogramModel(this->_histogramchart, hexeditdata, ui->lisOccurrence);
    ui->lisOccurrence->setModel(this->_histogrammodel);

    this->setEntropy("Calculating...", qApp->palette().text().color());

    this->_worker.setData(&this->_histogramchart, &this->_entropychart, hexeditdata);
    this->_worker.start(QThread::LowPriority);
}

ChartWidget::~ChartWidget()
{
    if(this->_worker.isRunning())
    {
        this->_worker.abort();
        this->_worker.wait();
    }

    delete ui;
}

void ChartWidget::onHistogramChartCompleted(quint64 size)
{
    this->_histogrammodel->updateStats();
    ui->chartcontainer->histogram()->setData(this->_histogramchart.result());

    for(int i = 0; i < this->_histogrammodel->columnCount(); i++)
        ui->lisOccurrence->resizeColumnToContents(i);

    ui->lisOccurrence->resizeRowsToContents();
    this->displayEntropy(this->_histogramchart.result(), size);
}

void ChartWidget::onEntropyChartCompleted(quint64 size)
{
    ui->chartcontainer->xyChart()->setXBase(16);
    ui->chartcontainer->xyChart()->setXRange(0, size);
    ui->chartcontainer->xyChart()->setYRange(0, 1);
    ui->chartcontainer->xyChart()->setPoints(this->_entropychart.points());
}

void ChartWidget::displayEntropy(const ByteElaborator::CountResult& cr, uint64_t size)
{
    double e = Algorithm::entropy(cr, size);
    this->setEntropy(QString::number(e), ByteColors::entropyColor(e));
}

void ChartWidget::setEntropy(const QString &text, const QColor &forecolor)
{
    ui->lblEntropy->setText(text);

    QPalette p = ui->lblEntropy->palette();
    p.setColor(ui->lblEntropy->foregroundRole(), forecolor);
    ui->lblEntropy->setPalette(p);
}

void ChartWidget::on_tbHelp_clicked()
{
    ByteColorsDialog bcd(this);
    bcd.exec();
}

void ChartWidget::on_tbSwitchChart_clicked()
{
    ui->chartcontainer->switchChart();

    if(ui->chartcontainer->histogram()->isVisible())
        ui->tbSwitchChart->setIcon(this->_xycharticon);
    else
        ui->tbSwitchChart->setIcon(this->_histogramicon);
}
