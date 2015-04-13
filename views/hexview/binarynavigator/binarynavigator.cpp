#include "binarynavigator.h"

const qreal BinaryNavigator::BYTES_PER_LINE = 0x10;

BinaryNavigator::BinaryNavigator(QWidget *parent): QGLWidget(parent), _hexeditdata(nullptr), _hexedit(nullptr), _size(0), _offset(0)
{
    this->_binarymap.setMode(BinaryMap::Class);
}

void BinaryNavigator::displayDefault()
{
    this->_binarymap.setMode(BinaryMap::Class);
    this->renderMap();
}

void BinaryNavigator::displayEntropy()
{
    this->_binarymap.setMode(BinaryMap::Entropy);
    this->renderMap();
}

void BinaryNavigator::setData(QHexEdit *hexedit)
{
    this->_hexedit = hexedit;
    this->_hexeditdata = hexedit->data();

    connect(this->_hexedit, SIGNAL(positionChanged(qint64)), this, SLOT(updateSquare(qint64)));
    this->renderMap();
}

void BinaryNavigator::renderMap(int)
{
    QDataBuffer databuffer(this->_hexeditdata);

    this->adjust();
    this->_binarymap.elaborate(&databuffer, this->_offset, this->_endoffset, BinaryNavigator::BYTES_PER_LINE);
    this->update();
}

qint64 BinaryNavigator::indexFromPoint(const QPoint &pt)
{
    qint64 y = pt.y() / this->_size;
    qint64 x = pt.x() / this->_size;

    return this->_hexedit->visibleStartOffset() + (x + (y * BinaryNavigator::BYTES_PER_LINE));
}

void BinaryNavigator::adjust()
{
    if(!this->_hexedit)
        return;

    uint64_t w = static_cast<uint64_t>(BinaryNavigator::BYTES_PER_LINE);

    this->_size = qFloor(static_cast<qreal>(this->width()) / BinaryNavigator::BYTES_PER_LINE);
    this->_offset = this->_hexedit->visibleStartOffset();
    this->_endoffset = std::min(static_cast<uint64_t>(this->_hexeditdata->length()), this->_offset + ((this->height() / this->_size) * w));
}

void BinaryNavigator::updateSquare(qint64)
{
    this->adjust();
    this->update();
}

void BinaryNavigator::wheelEvent(QWheelEvent *event)
{
    if(this->_hexedit && this->_hexeditdata)
        this->_hexedit->scroll(event); /* Forward Scroll */

    QGLWidget::wheelEvent(event);
}

void BinaryNavigator::mousePressEvent(QMouseEvent *event)
{
    if(this->_hexedit && ((event->modifiers() == Qt::NoModifier) && (event->button() == Qt::LeftButton)))
    {
        qint64 idx = this->indexFromPoint(event->pos());

        if(idx != -1)
            this->_hexedit->selectPos(idx);
    }
    else
        event->ignore();

    QGLWidget::mousePressEvent(event);
}

void BinaryNavigator::paintEvent(QPaintEvent *)
{
    if(!this->_hexeditdata || ! this->_hexedit || !this->isVisible())
        return;

    QRectF r(0, 0, this->_size, this->_size);
    QPainter p(this);

    const BinaryMap::ByteDataList& bdl = this->_binarymap.data();

    for(qreal i = 0; i < bdl.size(); i++, this->_offset++)
    {
        if(r.left() >= this->width())
            r.moveTo(0, r.top() + this->_size);

        QColor c;
        const BinaryMap::ByteData& bd = bdl.at(i);

        if(this->_binarymap.mode() == BinaryMap::Entropy)
            c = QColor::fromRgb(ByteColors::entropyColor(bd.Entropy));
        else
            c = QColor::fromRgb(ByteColors::categoryColor(bd.Category));

        p.fillRect(r, c);

        if(this->_offset == static_cast<uint64_t>(this->_hexedit->cursorPos()))
        {
            if(this->_binarymap.mode() == BinaryMap::Entropy)
                p.setPen(QColor(0xFF, 0xFF, 0x00));
            else
                p.setPen(QColor(0xFF, 0x00, 0xFF));

            p.drawRect(r);
        }

        r.moveLeft(r.left() + this->_size);
    }
}
