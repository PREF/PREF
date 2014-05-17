#include "displaymodewidget.h"
#include "ui_displaymodewidget.h"

DisplayModeWidget::DisplayModeWidget(QWidget *parent): QWidget(parent), ui(new Ui::DisplayModeWidget)
{
    ui->setupUi(this);

    ui->comboBox->addItem("Dot Plot", BinaryMap::DotPlot);
    ui->comboBox->addItem("Bytes as Pixel", BinaryMap::BytesAsPixel);
}

DisplayModeWidget::~DisplayModeWidget()
{
    delete ui;
}

void DisplayModeWidget::on_comboBox_currentIndexChanged(int index)
{
    BinaryMap::DisplayMode mode = static_cast<BinaryMap::DisplayMode>(ui->comboBox->itemData(index).toInt());
    emit displayModeChanged(mode);
}
