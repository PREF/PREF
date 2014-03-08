#include "actionwidget.h"

ActionWidget::ActionWidget(QWidget *parent): QWidget(parent)
{
    this->_findaction = new FindAction();
    this->_gotoaction = new GotoAction();
    this->_byteopsaction = new ByteOpsAction();
    this->_rangeselectoraction = new RangeSelectorAction();

    this->_hspacer = new QSpacerItem(40, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);

    this->_hlayout = new QHBoxLayout();
    this->_hlayout->setContentsMargins(0, 0, 0, 0);
    this->_hlayout->addWidget(this->_gotoaction);
    this->_hlayout->addWidget(this->_findaction);
    this->_hlayout->addWidget(this->_byteopsaction);
    this->_hlayout->addWidget(this->_rangeselectoraction);
    this->_hlayout->addItem(this->_hspacer);

    this->setLayout(this->_hlayout);
}

void ActionWidget::hideAllActions()
{
    this->hideAllBottomItems();
}

FindAction *ActionWidget::findAction()
{
    return this->_findaction;
}

GotoAction *ActionWidget::gotoAction()
{
    return this->_gotoaction;
}

ByteOpsAction *ActionWidget::byteOpsAction()
{
    return this->_byteopsaction;
}

RangeSelectorAction *ActionWidget::rangeSelectorAction()
{
    return this->_rangeselectoraction;
}

void ActionWidget::hideAllBottomItems(QWidget *ignore)
{
    if(ignore != this->_gotoaction)
        this->_gotoaction->hide();

    if(ignore != this->_findaction)
        this->_findaction->hide();

    if(ignore != this->_byteopsaction)
        this->_byteopsaction->hide();

    if(ignore != this->_rangeselectoraction)
        this->_rangeselectoraction->hide();

    if(ignore)
        emit actionDisplayed();
}

void ActionWidget::showFind()
{
    this->hideAllBottomItems(this->_findaction);

    if(!this->_findaction->isVisible())
        this->_findaction->show();
}

void ActionWidget::showGoto(qint64 max)
{
    this->hideAllBottomItems(this->_gotoaction);

    if(!this->_gotoaction->isVisible())
    {
        this->_gotoaction->setMaximum(max);
        this->_gotoaction->show();
    }
}

void ActionWidget::showByteOps()
{
    this->hideAllBottomItems(this->_byteopsaction);

    if(!this->_byteopsaction->isVisible())
        this->_byteopsaction->show();
}

void ActionWidget::showRangeSelector(qint64 max)
{
    this->hideAllBottomItems(this->_rangeselectoraction);

    if(!this->_rangeselectoraction->isVisible())
    {
        this->_rangeselectoraction->setMaximum(max);
        this->_rangeselectoraction->show();
    }
}
