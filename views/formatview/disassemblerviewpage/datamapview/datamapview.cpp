#include "datamapview.h"
#include "ui_datamapview.h"

DataMapView::DataMapView(QWidget *parent): QWidget(parent), ui(new Ui::DataMapView)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 1);
    ui->hexView->setReadOnly(true);

    this->_datamapmodel = new DataMapModel(ui->dataView);
    ui->dataView->setModel(this->_datamapmodel);
}

void DataMapView::setHexEditData(QHexEditData *hexeditdata)
{
    ui->hexView->setData(hexeditdata);
}

DataMapView::~DataMapView()
{
    delete ui;
}

void DataMapView::highlightData()
{
    ui->hexView->clearHighlight();

    /*
    for(quint64 i = 0; i < dl->dataCount(); i++)
    {
        DataInfo di = dl->data(i);
        ui->hexView->setRangeColor(di.Offset, (di.Offset + di.Size) - 1, QColor::fromRgb(qrand() % 255, qrand() % 255, qrand() % 255));
    }
    */
}
