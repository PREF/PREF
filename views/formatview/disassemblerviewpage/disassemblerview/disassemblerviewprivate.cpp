#include "disassemblerviewprivate.h"

DisassemblerViewPrivate::DisassemblerViewPrivate(QScrollArea* scrollarea, QScrollBar* vscrollbar, QWidget *parent): QWidget(parent), _hexeditdata(nullptr)
{
    this->_scrollArea = scrollarea;
    this->_vscrollbar = vscrollbar;
    this->_instructioncount = this->_charwidth = this->_charheight = this->_labelwidth = 0;

    this->_nofeaturecolor = Qt::lightGray;
    this->_callcolor = Qt::blue;
    this->_jumpcolor = Qt::darkRed;

    this->_digitcolor = Qt::darkBlue;
    this->_symbolcolor = Qt::darkCyan;
    this->_stopcolor = QColor::fromRgb(192, 88, 0);

    connect(this->_vscrollbar, SIGNAL(valueChanged(int)), this, SLOT(vScrollBarValueChanged(int)));

    /* Use Monospace Fonts! */
    QFont f("Monospace", qApp->font().pointSize());
    f.setStyleHint(QFont::TypeWriter); /* Use monospace fonts! */

    this->setFont(f);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setBackgroundRole(QPalette::Base);
}

void DisassemblerViewPrivate::setInstructionCount(quint64 instructioncount)
{
    this->_instructioncount = instructioncount;
}

void DisassemblerViewPrivate::setData(QHexEditData *hexeditdata)
{
    this->_hexeditdata = hexeditdata;
    this->_loadedformat = FormatList::loadedFormat(hexeditdata);

    this->adjust();
    this->update();
}

void DisassemblerViewPrivate::gotoVA(quint64 va)
{
    //if(this->_model && this->_model->itemCount())
    {
        /*
        qint64 residx;
        this->_listing->itemFromVA(va, &residx);

        if(residx != -1)
        {
            this->_vscrollbar->setSliderPosition(residx);
            this->update();
        }
        */
    }
}

void DisassemblerViewPrivate::gotoEP()
{
    /*
    if(this->_model && this->_model->itemCount())
    {
        quint64 idx = this->_model->entryPointIndex();

        if(idx != 0)
        {
            this->_vscrollbar->setSliderPosition(idx);
            this->update();
        }
    }
    */
}

void DisassemblerViewPrivate::setLoader(const ProcessorLoader &dl)
{
    this->_loader = dl;
}

void DisassemblerViewPrivate::adjust()
{
    QFontMetrics fm = this->fontMetrics();

    this->_charwidth = fm.width(" ");
    this->_charheight = fm.height();
    this->_labelwidth = this->_charwidth * 12;

    if(this->_hexeditdata)
    {
        /* Setup ScrollBars */
        qint64 totLines = this->_instructioncount;
        qint64 visLines = this->height() / this->_charheight;

        /* Setup Vertical ScrollBar */
        if(totLines > visLines)
        {
            this->_vscrollbar->setRange(0, (totLines - visLines) + 1);
            this->_vscrollbar->setSingleStep(1);
            this->_vscrollbar->setPageStep(visLines);
            this->_vscrollbar->show();
        }
        else
            this->_vscrollbar->hide();

        /* Setup Horizontal ScrollBar */
        //this->setMinimumWidth(this->_xPosend);
    }
    else
    {
        /* No File Loaded: Hide Scroll Bars */
        this->_vscrollbar->hide();
        this->setMinimumWidth(0);
    }

    this->update();
}

qint64 DisassemblerViewPrivate::verticalSliderPosition64()
{
    return static_cast<qint64>(this->_vscrollbar->sliderPosition());
}

bool DisassemblerViewPrivate::drawLine(DisassemblerViewDrawer* drawer, lua_Integer i)
{
    bool b = false;
    lua_State* l = SDKManager::state();

    lua_getglobal(l, "Sdk");
    lua_getfield(l, -1, "printInstruction");
    lua_pushlightuserdata(l, drawer);
    lua_pushlightuserdata(l, this->_hexeditdata);
    lua_pushinteger(l, i + 1);
    int res = lua_pcall(l, 3, 1, 0);

    if(res != 0)
        DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(l, -1)));
    else
        b = (lua_toboolean(l, -1) != 0);

    lua_pop(l, 2);
    return !res && b;
}

void DisassemblerViewPrivate::vScrollBarValueChanged(int)
{
    this->update();
}

void DisassemblerViewPrivate::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    if(this->_hexeditdata)
    {
        int y = 0;
        QRect r = e->rect();
        QFontMetrics fm = this->fontMetrics();
        qint64 slidepos = this->_vscrollbar->isVisible() ? this->verticalSliderPosition64() : 0;
        qint64 start = slidepos + (r.top() / this->_charheight), end = (slidepos + (r.bottom() / this->_charheight)) + 1; /* Removes the scroll bug */

        painter.setBackgroundMode(Qt::TransparentMode);

        DisassemblerViewDrawer dd(this->_hexeditdata, painter, fm, this->_charwidth, this->_charheight, y);

        for(qint64 i = start; i < end; i++)
        {            
            if(!this->drawLine(&dd, i))
                break;

            y += fm.height();
        }
    }
}

void DisassemblerViewPrivate::mousePressEvent(QMouseEvent *)
{

}

void DisassemblerViewPrivate::keyPressEvent(QKeyEvent *e)
{
    if(!this->_hexeditdata || !this->_instructioncount)
    {
        e->ignore();
        return;
    }

    if(e->matches(QKeySequence::MoveToNextPage) || e->matches(QKeySequence::MoveToPreviousPage))
    {
        qint64 newline = 0, linecount = this->height() / this->_charheight;

        if(e->matches(QKeySequence::MoveToPreviousPage))
        {
            newline = this->verticalSliderPosition64() - (linecount - 1);

            if(newline < 0)
                newline = 0;
        }
        else // if(e->matches(QKeySequence::MoveToNextPage))
        {
            newline = this->verticalSliderPosition64() + (linecount - 1);

            if(newline >= this->_instructioncount)
                newline = this->_instructioncount - 1;
        }

        this->_vscrollbar->setSliderPosition(newline);
    }
    else if(e->matches(QKeySequence::MoveToStartOfDocument) || e->matches(QKeySequence::MoveToEndOfDocument))
    {
        qint64 newline = 0;

        if(e->matches(QKeySequence::MoveToEndOfDocument))
            newline = this->_instructioncount - 1;

        this->_vscrollbar->setSliderPosition(newline);
    }

    e->accept();
    this->update();
}

void DisassemblerViewPrivate::resizeEvent(QResizeEvent*)
{
    if(this->_hexeditdata)
        this->adjust();
}

void DisassemblerViewPrivate::wheelEvent(QWheelEvent *event)
{
    if(this->_hexeditdata && this->_instructioncount)
    {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;

        if(event->orientation() == Qt::Vertical)
        {
            int pos = this->verticalSliderPosition64() - numSteps;
            int maxLines = this->_instructioncount;

             /* Bounds Check */
            if(pos < 0)
                pos = 0;
            else if(pos > maxLines)
                pos = maxLines;

            this->_vscrollbar->setSliderPosition(pos);
            this->update();

            event->accept();
        }
    }
    else
        event->ignore();
}
