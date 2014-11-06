#include "disassemblerwidget.h"

DisassemblerWidget::DisassemblerWidget(QWidget *parent): QFrame(parent)
{    
    this->_vscrollbar = new QScrollBar(Qt::Vertical);
    this->_scrollarea = new QScrollArea();
    this->_disasmwidget_p = new DisassemblerWidgetPrivate(this->_scrollarea, this->_vscrollbar);

    this->_scrollarea->setFocusPolicy(Qt::NoFocus);
    this->_scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); /* Do not show vertical QScrollBar!!! */
    this->_scrollarea->setFrameStyle(QFrame::NoFrame);
    this->_scrollarea->setWidgetResizable(true);
    this->_scrollarea->setWidget(this->_disasmwidget_p);

    this->setFocusPolicy(Qt::NoFocus);

    this->_hlayout = new QHBoxLayout();
    this->_hlayout->setSpacing(0);
    this->_hlayout->setMargin(0);
    this->_hlayout->addWidget(this->_scrollarea);
    this->_hlayout->addWidget(this->_vscrollbar);

    this->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    this->setLayout(this->_hlayout);

    /* Forward Signals */
    connect(this->_disasmwidget_p, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
    connect(this->_disasmwidget_p, SIGNAL(jumpToRequested()), this, SIGNAL(jumpToRequested()));
    connect(this->_disasmwidget_p, SIGNAL(crossReferenceRequested(Block*)), this, SIGNAL(crossReferenceRequested(Block*)));
    connect(this->_disasmwidget_p, SIGNAL(backAvailable(bool)), this, SIGNAL(backAvailable(bool)));
    connect(this->_disasmwidget_p, SIGNAL(forwardAvailable(bool)), this, SIGNAL(forwardAvailable(bool)));
}

void DisassemblerWidget::setDisassembler(DisassemblerDefinition *disassembler)
{
    this->_disasmwidget_p->setDisassembler(disassembler);
}

void DisassemblerWidget::setCurrentIndex(int idx)
{
    this->_disasmwidget_p->setCurrentIndex(idx);
}

void DisassemblerWidget::setListing(DisassemblerListing *listing)
{
    this->_disasmwidget_p->setListing(listing);
}

void DisassemblerWidget::setAddressForeColor(const QColor &c)
{
    this->_disasmwidget_p->setAddressForeColor(c);
}

void DisassemblerWidget::setSelectedLineColor(const QColor &c)
{
    this->_disasmwidget_p->setSelectedLineColor(c);
}

void DisassemblerWidget::setWheelScrollLines(int c)
{
    this->_disasmwidget_p->setWheelScrollLines(c);
}

void DisassemblerWidget::jumpTo(Block *block)
{
    this->_disasmwidget_p->jumpTo(block);
}

void DisassemblerWidget::jumpTo(const PrefSDK::DataValue& address)
{
    this->_disasmwidget_p->jumpTo(address);
}

void DisassemblerWidget::clearNavigationHistory()
{
    this->_disasmwidget_p->clearNavigationHistory();
}

void DisassemblerWidget::back()
{
    this->_disasmwidget_p->back();
}

void DisassemblerWidget::forward()
{
    this->_disasmwidget_p->forward();
}

qint64 DisassemblerWidget::currentIndex() const
{
    return this->_disasmwidget_p->currentIndex();
}

Block *DisassemblerWidget::selectedBlock() const
{
    return this->_disasmwidget_p->selectedBlock();
}
