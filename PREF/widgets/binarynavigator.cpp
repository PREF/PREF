#include "binarynavigator.h"
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

const uint64_t BinaryNavigator::BYTES_PER_LINE = 0x10;

BinaryNavigator::BinaryNavigator(QWidget *parent): QWidget(parent), _hexedit(NULL), _loadeddata(NULL), _squaresize(0), _offset(0)
{
    this->_binarymap.setMode(BinaryMap::Class);

    this->setMinimumWidth(128);
    this->setMaximumWidth(128);
}

void BinaryNavigator::displayDefault()
{
    this->_binarymap.setMode(BinaryMap::Class);
    this->renderMap();
}

void BinaryNavigator::switchView()
{
    BinaryMap::MapMode mode = (this->_binarymap.mode() == BinaryMap::Class) ? BinaryMap::Entropy : BinaryMap::Class;

    this->_binarymap.setMode(mode);
    this->renderMap();
}

void BinaryNavigator::initialize(QHexEdit *hexedit, LoadedData* loadeddata)
{
    this->_hexedit = hexedit;
    this->_loadeddata = loadeddata;

    connect(this->_hexedit->document()->cursor(), &QHexCursor::positionChanged, this, &BinaryNavigator::updateSquare);
    connect(this->_hexedit, &QHexEdit::verticalScroll, [this](integer_t) { this->renderMap(); });
    this->renderMap();
}

void BinaryNavigator::renderMap()
{
    if(!this->_loadeddata)
        return;

    this->adjust();
    this->_binarymap.elaborate(this->_loadeddata, this->_offset, this->_endoffset, BinaryNavigator::BYTES_PER_LINE);
    this->update();
}

integer_t BinaryNavigator::offsetFromPoint(const QPoint &pt)
{
    qint64 y = pt.y() / this->_squaresize;
    qint64 x = pt.x() / this->_squaresize;

    return this->_hexedit->metrics()->visibleStartOffset() + (x + (y * BinaryNavigator::BYTES_PER_LINE));
}

void BinaryNavigator::adjust()
{
    if(!this->_hexedit)
        return;

    this->_squaresize = qFloor(static_cast<qreal>(this->width()) / static_cast<qreal>(BinaryNavigator::BYTES_PER_LINE));
    this->_offset = this->_hexedit->metrics()->visibleStartOffset();
    this->_endoffset = qMin(this->_loadeddata->size(), this->_offset + ((this->height() / this->_squaresize) * BinaryNavigator::BYTES_PER_LINE));
}

void BinaryNavigator::updateSquare()
{
    this->adjust();
    this->update();
}

void BinaryNavigator::wheelEvent(QWheelEvent *event)
{
    if(this->_hexedit && this->_loadeddata)
        this->_hexedit->scroll(event); /* Forward Scroll */

    QWidget::wheelEvent(event);
}

void BinaryNavigator::mousePressEvent(QMouseEvent *event)
{
    if(this->_hexedit && ((event->modifiers() == Qt::NoModifier) && (event->button() == Qt::LeftButton)))
    {
        sinteger_t offset = this->offsetFromPoint(event->pos());
        QHexCursor* cursor = this->_hexedit->document()->cursor();
        cursor->setSelectionRange(offset, 1);
    }
    else
        event->ignore();

    QWidget::mousePressEvent(event);
}

void BinaryNavigator::paintEvent(QPaintEvent *)
{
    if(!this->_loadeddata || !this->_hexedit || !this->isVisible())
        return;

    const BinaryMap::ByteDataList& bdl = this->_binarymap.data();
    QHexCursor* cursor = this->_hexedit->document()->cursor();
    QRectF r(0, 0, this->_squaresize, this->_squaresize), cursorrect;
    QPainter p(this);

    for(integer_t i = 0; i < bdl.size(); i++, this->_offset++)
    {
        if(r.left() >= this->width())
            r.moveTo(0, r.top() + this->_squaresize);

        QColor c;
        const BinaryMap::ByteData& bd = bdl.at(i);

        if(this->_binarymap.mode() == BinaryMap::Entropy)
            c = QColor::fromRgb(ByteColors::entropyColor(bd.Entropy));
        else
            c = QColor::fromRgb(ByteColors::categoryColor(bd.Category));

        p.fillRect(r, c);

        if(this->_offset == cursor->offset())
            cursorrect = r;

        r.moveLeft(r.left() + this->_squaresize);
    }

    if(cursorrect.isEmpty())
        return;

    if(this->_binarymap.mode() == BinaryMap::Entropy)
        p.setPen(QColor(0xFF, 0xFF, 0x00));
    else
        p.setPen(QColor(0xFF, 0x00, 0xFF));

    p.drawRect(cursorrect);
}

void BinaryNavigator::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->renderMap();
}
