#include "disassemblerwidgetprivate.h"

DisassemblerWidgetPrivate::ListingItem DisassemblerWidgetPrivate::ListingItem::_invalidobj;

DisassemblerWidgetPrivate::DisassemblerWidgetPrivate(QScrollArea* scrollarea, QScrollBar* vscrollbar, QWidget *parent): QWidget(parent), _listing(nullptr), _currentindex(-1)
{
    this->_scrollarea = scrollarea;
    this->_vscrollbar = vscrollbar;
    this->_charwidth = this->_charheight = 0;

    QFont f("Monospace", qApp->font().pointSize());
    f.setStyleHint(QFont::TypeWriter); /* Use monospace fonts! */

    this->setFont(f);
    this->setWheelScrollLines(5);  /* By default: scroll 5 lines */
    this->setBackgroundRole(QPalette::Base);
    this->setAddressForeColor(Qt::darkBlue);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setSelectedLineColor(QColor(0xFF, 0xFF, 0xA0));

    connect(this->_vscrollbar, SIGNAL(valueChanged(int)), this, SLOT(onVScrollBarValueChanged(int)));
}

const DisassemblerWidgetPrivate::ListingItem &DisassemblerWidgetPrivate::selectedItem()
{
    if(this->_currentindex < this->_listingitems.count())
        return this->_listingitems[this->_currentindex];

    return ListingItem::invalid();
}

void DisassemblerWidgetPrivate::setCurrentIndex(int idx)
{
    this->_currentindex = idx;

    if(this->_listing)
        this->ensureVisible(idx);
}

void DisassemblerWidgetPrivate::setListing(DisassemblerListing *listing)
{
    this->_listing = listing;

    this->elaborateListing();
    this->adjust();
    this->update();
    this->gotoFirstEntryPoint();
}

void DisassemblerWidgetPrivate::setAddressForeColor(const QColor &c)
{
    this->_addressforecolor = c;
    this->update();
}

void DisassemblerWidgetPrivate::setSelectedLineColor(const QColor &c)
{
    this->_sellinecolor = c;
}

void DisassemblerWidgetPrivate::setWheelScrollLines(int c)
{
    this->_wheelscrolllines = c;
}

void DisassemblerWidgetPrivate::selectItem(ListingObject* listingobj)
{
    if(!this->_listingindexes.contains(listingobj))
        return;

    this->setCurrentIndex(this->_listingindexes[listingobj]);
}

void DisassemblerWidgetPrivate::gotoAddress(uint64_t address)
{
    for(int i = 0; i < this->_listing->segmentsCount(); i++)
    {
        Segment* segment = this->_listing->segment(i);

        if(segment->contains(address))
        {
            for(int j = 0; j < segment->functionsCount(); j++)
            {
                Function* func = segment->function(j);

                if(func->contains(address))
                {
                    for(int k = 0; k < func->instructionsCount(); k++)
                    {
                        Instruction* instruction = func->instruction(k);

                        if(instruction->contains(address))
                        {
                            this->selectItem(instruction);
                            return;
                        }
                    }
                }
                else if(func->endAddress() > address)
                    return;
            }
        }
        else if(segment->endAddress() > address)
            return;
    }
}

int DisassemblerWidgetPrivate::currentIndex() const
{
    return this->_currentindex;
}

void DisassemblerWidgetPrivate::gotoFirstEntryPoint()
{
    for(int i = 0; i < this->_listing->segmentsCount(); i++)
    {
        Segment* segment = this->_listing->segment(i);

        if(segment->entryPointsCount() > 0)
        {
            this->selectItem(segment->entryPoint(0));
            break;
        }
    }
}

QString DisassemblerWidgetPrivate::functionType(Function *f) const
{
    switch(f->type())
    {
        case FunctionTypes::EntryPoint:
            return "entrypoint";

        case FunctionTypes::Import:
            return "import";

        case FunctionTypes::Export:
            return "export";

        default:
            break;
    }

    return QString();
}

QString DisassemblerWidgetPrivate::displayReferences(const QString &prefix, const DisassemblerListing::ReferenceSet &references) const
{
    if(references.isEmpty())
        return QString();

    Segment* segment = nullptr;
    QString s = QString("# %1: ").arg(prefix);

    for(DisassemblerListing::ReferenceSet::ConstIterator it = references.begin(); it != references.end(); it++)
    {
        if(it != references.begin())
            s.append(", ");

        uint64_t srcaddress = (*it)->srcAddress();

        if(!segment || !segment->contains(srcaddress))
            segment = this->_listing->segmentFromAddress(srcaddress);

        if(segment)
            s.append(QString("%1:%2").arg(segment->name(), QString("%1").arg((*it)->srcAddress(), 8, 16, QLatin1Char('0')).toUpper()));
        else
            s.append(QString("%1").arg((*it)->srcAddress(), 8, 16, QLatin1Char('0')).toUpper());
    }

    return s;
}


void DisassemblerWidgetPrivate::drawLineBackground(QPainter &painter, qint64 idx, int y)
{
    QRect linerect(0, y, this->width(), this->_charheight);

    if(idx == this->_currentindex)
        painter.fillRect(linerect, this->_sellinecolor);
    else
        painter.fillRect(linerect, this->palette().color(QPalette::Base));
}

void DisassemblerWidgetPrivate::drawLine(QPainter &painter, QFontMetrics &fm, qint64 idx, int y)
{
    QTextDocument document;
    painter.setBackgroundMode(Qt::TransparentMode);
    this->drawLineBackground(painter, idx, y);

    ListingItem& listingitem = this->_listingitems[idx];

    if(listingitem.itemType() == ListingItem::Reference)
    {
        int x = 0;
        const DisassemblerListing::ReferenceSet& references = listingitem.references();
        document.setPlainText(this->emitReferences(fm, references, x));

        DisassemblerHighlighter highlighter(&document, references);
        highlighter.rehighlight();  /* Apply Syntax Highlighting */

        painter.save();
            painter.translate(x + (this->_charwidth * 4), y - (this->_charheight - fm.ascent()));
            document.drawContents(&painter);
        painter.restore();

        return;
    }

    ListingObject* listingobj = listingitem.listingObject();
    int x = this->drawAddress(painter, fm, listingobj, y);

    switch(listingobj->objectType())
    {
        case ListingTypes::Segment:
            document.setPlainText(this->emitSegment(qobject_cast<Segment*>(listingobj)));
            break;

        case ListingTypes::Function:
            document.setPlainText(this->emitFunction(qobject_cast<Function*>(listingobj)));
            x += this->_charwidth * 2;
            break;

        case ListingTypes::Instruction:
            document.setPlainText(this->emitInstruction(qobject_cast<Instruction*>(listingobj)));
            x += this->_charwidth * 5;
            break;

        default:
            return;
    }

    DisassemblerHighlighter highlighter(&document, listingobj);
    highlighter.rehighlight();  /* Apply Syntax Highlighting */

    painter.save();
        painter.translate(x, y - (this->_charheight - fm.ascent()));
        document.drawContents(&painter);
    painter.restore();
}

QString DisassemblerWidgetPrivate::emitSegment(Segment *segment)
{
    QString startaddress = QString("%1").arg(segment->startAddress(), 8, 16, QLatin1Char('0')).toUpper();
    QString endaddress = QString("%1").arg(segment->endAddress(), 8, 16, QLatin1Char('0')).toUpper();

    return QString("segment '%1' (Start Address: %2h, End Address: %3h)").arg(segment->name(), startaddress, endaddress);
}

QString DisassemblerWidgetPrivate::emitFunction(Function* func)
{
    DisassemblerListing::ReferenceSet references = this->_listing->references(func->startAddress());
    SymbolTable symboltable = this->_listing->symbolTable();

    return QString("%1 function %2()\t %3").arg(this->functionType(func), symboltable[func->startAddress()]->name(), this->displayReferences("Called by", references));
}

QString DisassemblerWidgetPrivate::emitInstruction(Instruction *instruction)
{
    return QString("%1 %2").arg(instruction->mnemonic(), instruction->displayOperands());
}

QString DisassemblerWidgetPrivate::emitReferences(QFontMetrics& fm, const DisassemblerListing::ReferenceSet &references, int& x)
{
    Reference* r = *(references.begin());
    x = this->calcAddressWidth(fm, r);

    QString address = QString("%1").arg(static_cast<quint64>(r->destAddress()), 8, 16, QLatin1Char('0')).toUpper();
    return QString("j_%1:\t\t%2").arg(address, this->displayReferences("Referenced by", references));
}

void DisassemblerWidgetPrivate::elaborateListing()
{
    int line = 0;
    this->_listingitems.clear();
    this->_listingindexes.clear();

    for(int i = 0; i < this->_listing->segmentsCount(); i++)
    {
        Segment* segment = this->_listing->segment(i);

        this->_listingitems.append(ListingItem(segment));
        this->_listingindexes[segment] = line;
        line++;

        for(int j = 0; j < segment->functionsCount(); j++)
        {
            Function* func = segment->function(j);
            this->_listingitems.append(ListingItem(func));
            this->_listingindexes[func] = line;
            line++;

            for(int k = 0; k < func->instructionsCount(); k++)
            {
                Instruction* instr = func->instruction(k);
                DisassemblerListing::ReferenceSet references = this->_listing->references(instr->address());

                if(references.count() && ((*references.begin())->type() == ReferenceTypes::Jump || (*references.begin())->type() == ReferenceTypes::ConditionalJump))
                {
                    this->_listingitems.append(ListingItem(references));
                    line++;
                }

                this->_listingitems.append(ListingItem(instr));
                this->_listingindexes[instr] = line;
                line++;
            }
        }
    }
}

int DisassemblerWidgetPrivate::drawAddress(QPainter &painter, QFontMetrics &fm, ListingObject *listingobj, int y)
{
    QString address = QString("%1:%2").arg(listingobj->segmentName(), listingobj->displayAddress());
    int w = fm.width(address);

    painter.setPen(this->_addressforecolor);
    painter.drawText(0, y, w, this->_charheight, Qt::AlignLeft | Qt::AlignTop, address);

    return w + this->_charwidth;
}

int DisassemblerWidgetPrivate::calcAddressWidth(QFontMetrics &fm, Reference *reference)
{
    Segment* segment = this->_listing->segmentFromAddress(reference->srcAddress());

    if(!segment)
        return 0;

    return fm.width(QString("%1:%2").arg(segment->name(), reference->displayAddress()));
}

void DisassemblerWidgetPrivate::adjust()
{
    QFontMetrics fm = this->fontMetrics();

    this->_charwidth = fm.width(" ");
    this->_charheight = fm.height();

    if(this->_listing)
    {
        qint64 vislines = this->height() / this->_charheight;

        /* Setup Vertical ScrollBar */
        if(this->_listingitems.length() > vislines)
        {
            this->_vscrollbar->setRange(0, (this->_listingitems.length() - vislines) + 1);
            this->_vscrollbar->setSingleStep(1);
            this->_vscrollbar->setPageStep(vislines);
            this->_vscrollbar->show();
        }
        else
            this->_vscrollbar->hide();
    }
    else
    {
        /* No File Loaded: Hide Scroll Bars */
        this->_vscrollbar->hide();
        this->setMinimumWidth(0);
    }
}

void DisassemblerWidgetPrivate::ensureVisible(int idx)
{
    if(idx == -1)
        idx = 0;

    int firstidx = this->_vscrollbar->sliderPosition();
    int lastidx = firstidx + (this->height() / this->_charheight);

    if((idx < firstidx) || (idx > lastidx))
        this->_vscrollbar->setValue(idx);

    this->update();
}

void DisassemblerWidgetPrivate::onVScrollBarValueChanged(int)
{
    this->update();
}

void DisassemblerWidgetPrivate::wheelEvent(QWheelEvent *e)
{
    if(this->_listing)
    {
        int numDegrees = e->delta() / 8;
        int numSteps = numDegrees / 15;

        if(e->orientation() == Qt::Vertical)
        {
            int pos = this->_vscrollbar->sliderPosition() - (numSteps * this->_wheelscrolllines);

            /* Bounds Check */
            if(pos < 0)
                pos = 0;
            else if(pos > this->_listingitems.length())
                pos = this->_listingitems.length() - 1;

            this->_vscrollbar->setSliderPosition(pos);
            this->update();

            e->accept();
        }
    }

    QWidget::wheelEvent(e);
}

void DisassemblerWidgetPrivate::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);

    if(this->_listing)
    {
        QRect r = pe->rect();
        QFontMetrics fm = this->fontMetrics();
        qint64 slidepos = this->_vscrollbar->isVisible() ?this->_vscrollbar->sliderPosition() : 0;
        qint64 start = slidepos + (r.top() / this->_charheight), end = qMin(static_cast<qint64>(this->_listingitems.length() - 1), (slidepos + (r.bottom() / this->_charheight)) + 1); /* end + 1 Removes the scroll bug */

        for(qint64 i = start; i <= end; i++)
        {
            int y = (i - slidepos) * this->_charheight;
            this->drawLine(painter, fm, i, y);
        }
    }
}

void DisassemblerWidgetPrivate::resizeEvent(QResizeEvent *)
{
    this->adjust(); /* Update ScrollBars */
}

void DisassemblerWidgetPrivate::mousePressEvent(QMouseEvent *e)
{
    if(this->_listing)
    {
        QPoint pos = e->pos();

        if(pos.x() && pos.y())
            this->setCurrentIndex(this->_vscrollbar->sliderPosition() + (pos.y() / this->_charheight));
    }
    else
        this->setCurrentIndex(-1);

    QWidget::mousePressEvent(e);
}
