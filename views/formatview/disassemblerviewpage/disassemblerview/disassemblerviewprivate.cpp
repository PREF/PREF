#include "disassemblerviewprivate.h"

DisassemblerViewPrivate::DisassemblerViewPrivate(QScrollArea* scrollarea, QScrollBar* vscrollbar, QWidget *parent): QWidget(parent)
{
    this->_scrollArea = scrollarea;
    this->_vscrollbar = vscrollbar;
    this->_charwidth = this->_charheight = this->_labelwidth = 0;

    this->_nofeaturecolor = Qt::lightGray;
    this->_callcolor = Qt::blue;
    this->_jumpcolor = Qt::darkRed;

    this->_digitcolor = Qt::darkBlue;
    this->_symbolcolor = Qt::darkCyan;
    this->_stopcolor = QColor::fromRgb(192, 88, 0);

    connect(this->_vscrollbar, SIGNAL(valueChanged(int)), this, SLOT(vScrollBarValueChanged(int)));

    /* Use Monospace Fonts! */
    QFont f("Monospace", 10);

    this->setFont(f);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setBackgroundRole(QPalette::Base);
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

void DisassemblerViewPrivate::setLoader(DisassemblerLoader::Ptr dl)
{
    this->_loader = dl;
}

void DisassemblerViewPrivate::setListing(DisassemblerListing::Ptr dl)
{
    this->_listing = dl;

    this->adjust();
    this->update();
}

void DisassemblerViewPrivate::adjust()
{
    QFontMetrics fm = this->fontMetrics();

    this->_charwidth = fm.width(" ");
    this->_charheight = fm.height();
    this->_labelwidth = this->_charwidth * 12;
    this->_hexdumpwidth = this->_charwidth * (this->_listing->maxInstructionSize() * 3);

    if(this->_listing)
    {
        /* Setup ScrollBars */
        qint64 totLines = this->_listing->itemCount();
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

void DisassemblerViewPrivate::setCharColor(InstructionItem* ii, const QChar& ch, QPainter &painter)
{
    this->setInstructionColor(ii, painter);

    if(ch.isSymbol() || ch.isPunct())
        painter.setPen(this->_symbolcolor);
}

void DisassemblerViewPrivate::setInstructionColor(InstructionItem *ii, QPainter &painter)
{
    Instruction::Ptr instruction = ii->instruction();
    Instruction::InstructionFeatures features = this->_loader->processor()->features(instruction->instructionType());

    if(features & Instruction::Jump)
        painter.setPen(this->_jumpcolor);
    else if(features & Instruction::Call)
        painter.setPen(this->_callcolor);
    else if(features & Instruction::Stop)
        painter.setPen(this->_stopcolor);
    else if(features == 0)
        painter.setPen(this->_nofeaturecolor);
    else
        painter.setPen(Qt::black);
}

void DisassemblerViewPrivate::drawLine(QPainter &painter, QFontMetrics &fm, lua_Integer i, int y)
{
    int x = 0;
    ListingItem* item = this->_listing->item(i);

    if(this->_loader->inSegment(item->address()))
    {
        DisassemblerSegment segment = this->_loader->segment(item->address());
        x = this->drawAddress(segment.name(), painter, fm, item, y);
    }
    else
        x = this->drawAddress("???", painter, fm, item, y);

    if(item->itemType() == ListingItem::Instruction)
    {
        InstructionItem* ii = dynamic_cast<InstructionItem*>(item);
        this->drawHexDump(ii, painter, fm, x, y);
        this->drawInstruction(ii, painter, fm, x + this->_hexdumpwidth + this->_labelwidth, y);
    }
    else if(item->itemType() == ListingItem::Label)
        this->drawLabel(dynamic_cast<LabelItem*>(item), painter, fm, x + this->_hexdumpwidth, y);
}

int DisassemblerViewPrivate::drawAddress(const QString& segmentname, QPainter &painter, QFontMetrics &fm, ListingItem* li, int y)
{
    QString vaaddr = QString("%1:%2").arg(segmentname, QString("%1").arg(li->address(), 8, 16, QLatin1Char('0')).toUpper());

    painter.setPen(Qt::darkBlue);
    painter.drawText(0, y, fm.width(vaaddr), this->_charheight, Qt::AlignLeft | Qt::AlignTop, vaaddr);

    return fm.width(vaaddr) + this->_charwidth;
}

void DisassemblerViewPrivate::drawHexDump(InstructionItem *ii, QPainter &painter, QFontMetrics &fm, int x, int y)
{
    ByteBuffer* bytebuffer = this->_listing->buffer();
    QByteArray ba = bytebuffer->read(ii->address(), ii->instruction()->instructionSize());
    QString hexdump;

    for(int i = 0; i < ba.length(); i++)
    {
        if(!hexdump.isEmpty())
            hexdump.append(" ");

        hexdump.append(QString("%1").arg(static_cast<unsigned char>(ba.at(i)), 2, 16, QLatin1Char('0')).toUpper());
    }

    painter.setPen(Qt::darkGray);
    painter.drawText(x, y, fm.width(hexdump), this->_charheight, Qt::AlignLeft | Qt::AlignTop, hexdump);
}

void DisassemblerViewPrivate::drawLabel(LabelItem *li, QPainter &painter, QFontMetrics &fm, int x, int y)
{
    ReferenceTable* reftable = this->_listing->referenceTable();
    ReferenceTable::Reference::Ptr ref = reftable->reference(li->address());
    QString s = li->stringValue();
    int w = fm.width(s);

    painter.setPen(Qt::darkCyan);
    painter.drawText(x, y, w, this->_charheight, Qt::AlignLeft | Qt::AlignTop, s);

    if(ref)
        this->drawReference(ref, li->address(), painter, fm, x + w + this->_labelwidth, y);
}

void DisassemblerViewPrivate::drawComment(const QString &s, QPainter &painter, QFontMetrics &fm, int x, int y)
{
    if(!s.isEmpty())
    {
        painter.setPen(Qt::darkGreen);
        painter.drawText(x, y, fm.width(s), this->_charheight, Qt::AlignLeft | Qt::AlignTop, s);
    }
}

void DisassemblerViewPrivate::drawInstruction(InstructionItem* ii, QPainter &painter, QFontMetrics &fm, int x, int y)
{
    QString instr = ii->stringValue();

    for(int i = 0; i < instr.length(); i++)
    {
        QChar ch = instr.at(i);
        this->setCharColor(ii, ch, painter);

        painter.drawText(x, y, fm.width(ch), this->_charheight, Qt::AlignLeft | Qt::AlignTop, ch);
        x += fm.width(ch);
    }
}

void DisassemblerViewPrivate::drawReference(const ReferenceTable::Reference::Ptr ref, lua_Integer ignoreaddress, QPainter &painter, QFontMetrics &fm, int x, int y)
{
    QString addresses;

    for(int i = 0; i < ref->InstructionAddresses.length(); i++)
    {
        lua_Integer address = ref->InstructionAddresses[i];

        if(address == ignoreaddress)
            continue;

        if(this->_loader->inSegment(address))
        {
            DisassemblerSegment ds = this->_loader->segment(address);

            if(addresses.isEmpty())
                addresses.append(QString("%1:%2").arg(ds.name(), QString::number(address, 16).toUpper()));
            else
                addresses.append(QString(" | %1:%2").arg(ds.name(), QString::number(address, 16).toUpper()));
        }
    }

    if(!addresses.isEmpty())
    {
        QString reftype;

        if(ref->Type == ReferenceTable::Code)
            reftype = "CODE";
        else
            reftype = "DATA";

        QString s = QString("%1 XREF: %2").arg(reftype, addresses);

        painter.setPen(Qt::darkGreen);
        painter.drawText(x, y, fm.width(s), this->_charheight, Qt::AlignLeft | Qt::AlignTop, s);
    }
}

void DisassemblerViewPrivate::onDisassemblerDataChanged(quint64)
{
    this->update();
    this->adjust();  /* Update ScrollBars */
}

void DisassemblerViewPrivate::vScrollBarValueChanged(int)
{
    this->update();
}

void DisassemblerViewPrivate::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    if(this->_listing && this->_listing->itemCount())
    {
        int y = 0;
        QRect r = e->rect();
        QFontMetrics fm = this->fontMetrics();
        qint64 slidepos = this->_vscrollbar->isVisible() ? this->verticalSliderPosition64() : 0;
        qint64 start = slidepos + (r.top() / this->_charheight), end = (slidepos + (r.bottom() / this->_charheight)) + 1; /* Removes the scroll bug */

        painter.setBackgroundMode(Qt::TransparentMode);

        for(qint64 i = start; (i < this->_listing->itemCount()) && (i < end); i++)
        {
            this->drawLine(painter, fm, i, y);
            y += fm.height();
        }
    }
}

void DisassemblerViewPrivate::mousePressEvent(QMouseEvent *)
{

}

void DisassemblerViewPrivate::keyPressEvent(QKeyEvent *e)
{
    if(!this->_listing || !this->_listing->itemCount())
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

            if(newline >= this->_listing->itemCount())
                newline = this->_listing->itemCount() - 1;
        }

        this->_vscrollbar->setSliderPosition(newline);
    }
    else if(e->matches(QKeySequence::MoveToStartOfDocument) || e->matches(QKeySequence::MoveToEndOfDocument))
    {
        qint64 newline = 0;

        if(e->matches(QKeySequence::MoveToEndOfDocument))
            newline = this->_listing->itemCount() - 1;

        this->_vscrollbar->setSliderPosition(newline);
    }

    e->accept();
    this->update();
}

void DisassemblerViewPrivate::resizeEvent(QResizeEvent*)
{
    if(this->_listing)
        this->adjust();
}

void DisassemblerViewPrivate::wheelEvent(QWheelEvent *event)
{
    if(this->_listing && this->_listing->itemCount())
    {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;

        if(event->orientation() == Qt::Vertical)
        {
            int pos = this->verticalSliderPosition64() - numSteps;
            int maxLines = this->_listing->itemCount();

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
