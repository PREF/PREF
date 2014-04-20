#include "disassemblerview.h"

DisassemblerView::DisassemblerView(QWidget *parent): QFrame(parent)
{
    this->_vscrollbar = new QScrollBar(Qt::Vertical);
    this->_scrollarea = new QScrollArea();
    this->_disasmview_p = new DisassemblerViewPrivate(this->_scrollarea, this->_vscrollbar);

    this->_scrollarea->setFocusPolicy(Qt::NoFocus);
    this->_scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); /* Do not show vertical QScrollBar!!! */
    this->_scrollarea->setFrameStyle(QFrame::NoFrame);
    this->_scrollarea->setWidgetResizable(true);
    this->_scrollarea->setWidget(this->_disasmview_p);

    this->setFocusPolicy(Qt::NoFocus);

    this->_hlayout = new QHBoxLayout();
    this->_hlayout->setSpacing(0);
    this->_hlayout->setMargin(0);
    this->_hlayout->addWidget(this->_scrollarea);
    this->_hlayout->addWidget(this->_vscrollbar);

    this->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    this->setLayout(this->_hlayout);
}

void DisassemblerView::setLoader(const ProcessorLoader &dl)
{
    this->_disasmview_p->setLoader(dl);
}

void DisassemblerView::setListing(DisassemblerListing::Ptr dl)
{
    this->_disasmview_p->setListing(dl);
}

void DisassemblerView::gotoVA(quint64 va)
{
    this->_disasmview_p->gotoVA(va);
}

void DisassemblerView::gotoEP()
{
    this->_disasmview_p->gotoEP();
}
