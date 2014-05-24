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
}

void DisassemblerWidget::setInstructionCount(quint64 instructioncount)
{
    this->_disasmwidget_p->setInstructionCount(instructioncount);
}

void DisassemblerWidget::setData(QHexEditData *hexeditdata)
{
    this->_disasmwidget_p->setData(hexeditdata);
}

void DisassemblerWidget::setLoader(const ProcessorLoader &dl)
{
    this->_disasmwidget_p->setLoader(dl);
}

void DisassemblerWidget::gotoVA(quint64 va)
{
    this->_disasmwidget_p->gotoVA(va);
}

void DisassemblerWidget::gotoEP()
{
    this->_disasmwidget_p->gotoEP();
}
