#include "rangeselectoraction.h"
#include "ui_rangeselectoraction.h"

RangeSelectorAction::RangeSelectorAction(QWidget *parent): QWidget(parent), ui(new Ui::RangeSelectorAction)
{
    ui->setupUi(this);
    ui->sbbsFrom->setMinimum(0);
    ui->sbbsTo->setMinimum(0);

    this->setVisible(false);

    connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ui->sbbsFrom, SIGNAL(valueChanged(qint64)), this, SLOT(onValueChanged(qint64)));
    connect(ui->sbbsTo, SIGNAL(valueChanged(qint64)), this, SLOT(onValueChanged(qint64)));
}

RangeSelectorAction::~RangeSelectorAction()
{
    delete ui;
}

void RangeSelectorAction::setMaximum(qint64 val)
{
    ui->sbbsFrom->setMaximum(val);
    ui->sbbsTo->setMaximum(val);
}

void RangeSelectorAction::on_pbSelect_clicked()
{
    emit rangeSelected(ui->sbbsFrom->value(), ui->sbbsTo->value());
}

void RangeSelectorAction::onValueChanged(qint64)
{
    if(ui->sbbsFrom->value() < ui->sbbsTo->value())
        ui->pbSelect->setEnabled(true);
    else
        ui->pbSelect->setEnabled(false);
}

void RangeSelectorAction::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    ui->sbbsFrom->clear();
    ui->sbbsTo->clear();
}
