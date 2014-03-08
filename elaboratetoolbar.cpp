#include "elaboratetoolbar.h"

QIcon ElaborateToolBar::_icostart;
QIcon ElaborateToolBar::_icostop;
QString ElaborateToolBar::_starttext;
QString ElaborateToolBar::_stoptext;

ElaborateToolBar::ElaborateToolBar(QWidget *parent): QToolBar(parent)
{
    if(_icostart.isNull())
        _icostart = QIcon(":/misc_icons/res/start.png");

    if(_icostop.isNull())
        _icostop = QIcon(":/misc_icons/res/stop.png");

    if(_starttext.isEmpty())
        _starttext = "Start";

    if(_stoptext.isEmpty())
        _stoptext = "Stop";

    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->_actelaborate = this->addAction(QString());
    this->morphToStart();

    connect(this->_actelaborate, SIGNAL(triggered()), this, SLOT(onElaborateTriggered()));
}

void ElaborateToolBar::setElaborateEnabled(bool e)
{
    this->_actelaborate->setEnabled(e);
}

void ElaborateToolBar::morphToStart()
{
    if(this->isStart())
        return;

    this->_actelaborate->setIcon(ElaborateToolBar::_icostart);
    this->_actelaborate->setText(ElaborateToolBar::_starttext);
}

void ElaborateToolBar::morphToStop()
{
    if(!this->isStart())
        return;

    this->_actelaborate->setIcon(ElaborateToolBar::_icostop);
    this->_actelaborate->setText(ElaborateToolBar::_stoptext);
}

bool ElaborateToolBar::isStart()
{
    return (QString::compare(this->_actelaborate->text(), ElaborateToolBar::_starttext, Qt::CaseInsensitive) == 0);
}

void ElaborateToolBar::elaborationCompleted()
{
    this->morphToStart();
}

void ElaborateToolBar::onElaborateTriggered()
{
    if(this->isStart())
    {
        this->morphToStop();
        emit startTriggered();
    }
    else
    {
        this->morphToStart();
        emit stopTriggered();
    }
}
