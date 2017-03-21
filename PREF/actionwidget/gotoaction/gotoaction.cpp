#include "gotoaction.h"
#include "ui_gotoaction.h"

GotoAction::GotoAction(QWidget *parent): AbstractAction(parent), ui(new Ui::GotoAction)
{
    ui->setupUi(this);
    ui->sbbsAddress->setMinimum(0);

    this->setVisible(false);
    connect(ui->sbbsAddress, SIGNAL(valueChanged(qint64)), this, SIGNAL(addressChanged(qint64)));
    connect(ui->pbClose, SIGNAL(clicked()), this, SIGNAL(closeRequested()));
}

GotoAction::~GotoAction()
{
    delete ui;
}

void GotoAction::setMinimum(qint64 v)
{
    ui->sbbsAddress->setMinimum(v);
}

void GotoAction::setMaximum(qint64 v)
{
    ui->sbbsAddress->setMaximum(v);
}

void GotoAction::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    ui->sbbsAddress->clear();
}
