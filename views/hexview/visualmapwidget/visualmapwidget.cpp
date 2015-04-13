#include "visualmapwidget.h"
#include "ui_visualmapwidget.h"

VisualMapWidget::VisualMapWidget(QWidget *parent): QWidget(parent), ui(new Ui::VisualMapWidget), _visualmapmodel(nullptr), _visualmapdelegate(nullptr), _hexedit(nullptr)
{
    ui->setupUi(this);

    connect(ui->binaryMap, SIGNAL(gotoTriggered(qint64)), this, SIGNAL(gotoTriggered(qint64)));
}

void VisualMapWidget::map(QHexEdit *hexedit)
{
    this->_hexedit = hexedit;

    ui->binaryMap->setDisplayMode(BinaryMapWidget::DotPlot);
    ui->binaryMap->setData(hexedit);

    this->_visualmapmodel = new VisualMapModel(ui->mapOptions);
    this->_visualmapdelegate = new VisualMapDelegate(hexedit->data(), this);
    ui->mapOptions->setModel(this->_visualmapmodel);
    ui->mapOptions->setItemDelegate(this->_visualmapdelegate);

    connect(hexedit, SIGNAL(verticalScrollBarValueChanged(int)), this, SLOT(updateOffset(int)));
    connect(ui->binaryMap, SIGNAL(offsetChanged(qint64)), this->_visualmapmodel, SLOT(setCurrentOffset(qint64)));

    connect(this->_visualmapmodel, SIGNAL(widthChanged(qint64)), ui->binaryMap, SLOT(setWidth(qint64)));
    connect(this->_visualmapmodel, SIGNAL(displayModeChanged(BinaryMapWidget::DisplayMode)), ui->binaryMap, SLOT(setDisplayMode(BinaryMapWidget::DisplayMode)));

    ui->mapOptions->resizeRowsToContents();
}

VisualMapWidget::~VisualMapWidget()
{
    delete ui;
}

void VisualMapWidget::updateOffset(int)
{
    this->_visualmapmodel->setStartOffset(this->_hexedit->visibleStartOffset());
}
