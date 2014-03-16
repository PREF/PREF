#include "binarymap.h"

const QString BinaryMap::NO_DATA_AVAILABLE = "No Data Available";

BinaryMap::BinaryMap(QWidget *parent): QGLWidget(parent), _displaymode(BinaryMap::None), _bytebuffer(nullptr), _step(0), _start(-1), _end(-1), _width(-1)
{
    QFont f("Monospace", 10);

    this->setMouseTracking(true);
    this->setCursor(QCursor(Qt::CrossCursor));
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFont(f);

    QPalette p = this->palette();
    p.setColor(QPalette::Background, QColor(Qt::black));

    this->setPalette(p);
}

void BinaryMap::setStart(int start)
{
    if(start < 0)
        start = 0;
    else if((this->_end != -1) && (start >= this->_end))
        start = this->_end - 1;

    this->_start = start;

    if(this->_bytebuffer)
        this->update();
}

void BinaryMap::setEnd(int end)
{
    if((this->_start != -1) && (end < this->_start))
        end =  this->_start + 1;
    else if(this->_bytebuffer && end >= this->_bytebuffer->length())
        end = this->_bytebuffer->length();

    this->_end = end;

    if(this->_bytebuffer)
        this->update();
}

void BinaryMap::setWidth(int w)
{
    if(w < 0)
        w = 0;
    else if(w > this->width())
        w = this->width();

    this->_width = w;

    if(this->_bytebuffer)
        this->update();
}

void BinaryMap::setStep(qint64 step)
{
    this->_step = step;
}

void BinaryMap::setDisplayMode(BinaryMap::DisplayMode mode)
{
    this->_displaymode = mode;

    if(this->_bytebuffer)
        this->update();
}

void BinaryMap::setData(ByteBuffer *bytebuffer)
{
    this->_bytebuffer = bytebuffer;
}

qint64 BinaryMap::start()
{
    return qMin(this->_start, this->_end);
}

qint64 BinaryMap::end()
{
    return qMax(this->_start, this->_end);
}

qint64 BinaryMap::preferredHeight()
{
    return qMin((this->end() - this->start()) / this->_width, static_cast<qint64>(this->height()));
}

QByteArray BinaryMap::data()
{
    qint64 s = this->start();
    qint64 l = qMin(static_cast<qint64>(this->_bytebuffer->length()), this->_width * this->preferredHeight());
    return this->_bytebuffer->read(s, l);
}

qint64 BinaryMap::calcOffset(const QPoint &cursorpos)
{
   qint64 offset = -1;

   if(this->_displaymode == BinaryMap::DotPlot)
       offset = this->calcDotPlotOffset(cursorpos);
   else if(this->_displaymode == BinaryMap::ByteClass)
       offset = this->calcByteClassOffset(cursorpos);

   return offset;
}

qint64 BinaryMap::calcDotPlotOffset(const QPoint &cursorpos)
{
    qint64 s = qMin(static_cast<lua_Integer>(500), this->_bytebuffer->length());

    if(cursorpos.x() > s || cursorpos.y() > s)
        return -1;

    qint64 offset = this->_start + (cursorpos.x() + (cursorpos.y() / s));

    if((offset >= 0) && (offset <= this->_end))
        return offset;

    return -1;
}

qint64 BinaryMap::calcByteClassOffset(const QPoint &cursorpos)
{
    if(cursorpos.x() > this->_width)
        return -1;

    qint64 offset = this->_start + (cursorpos.x() + (cursorpos.y() * this->_width));

    if((offset >= 0) && (offset <= this->_end))
        return offset;

    return -1;
}

void BinaryMap::renderByteClassMap(QPainter& p)
{
    this->_bits = this->data();

    QImage img(reinterpret_cast<const uchar*>(this->_bits.constData()), this->_width, this->preferredHeight(), this->_width, QImage::Format_Indexed8);
    img.setColorTable(ByteColors::byteClassColorTable());
    p.drawImage(0, 0, img);
}

void BinaryMap::renderDotPlotMap(QPainter& p)
{
    qint64 s = qMin(static_cast<lua_Integer>(500), this->_bytebuffer->length());

    QImage img(s, s, QImage::Format_RGB888);
    img.fill(QColor(Qt::black));

    for(qint64 i = 0; i < s; i++)
    {
        if((this->start() + i) >= this->_bytebuffer->length())
            break;

        uchar ib = this->_bytebuffer->at(this->start() + i);

        for(qint64 j = 0; j < s; j++)
        {
            if((this->start() + j) >= this->_bytebuffer->length())
                break;

            uchar jb = this->_bytebuffer->at(this->start() + j);

            if(jb == ib)
                img.setPixel(j, i, qRgb(0, static_cast<uchar>((static_cast<qreal>(jb) * 0.75) + 64), 0));
        }
    }

    p.drawImage(0, 0, img);
}

void BinaryMap::drawNoDataAvailable(QPainter &p)
{
    QFontMetrics fm = this->fontMetrics();

    p.setPen(QColor(Qt::white));
    p.drawText(10, 10, fm.width(BinaryMap::NO_DATA_AVAILABLE), fm.height(), Qt::AlignLeft | Qt::AlignTop, BinaryMap::NO_DATA_AVAILABLE);
}

void BinaryMap::drawInfo(QPainter &p)
{
    QString start = QString("Start: %1").arg(QString("%1").arg(this->start(), 8, 16, QLatin1Char('0')));
    QString end = QString("End: %1").arg(QString("%1").arg(this->end(), 8, 16, QLatin1Char('0')));
    QString width = QString("Width: %1").arg(this->_width);

    QFontMetrics fm = this->fontMetrics();
    int maxlen = qMax(start.length(), qMax(end.length(), width.length())) + 1;
    int maxw = fm.maxWidth() * maxlen;

    QRect r = QRect(this->width() - maxw, 10, maxw, fm.height() * 3);

    p.setFont(this->font());
    p.setPen(QColor(Qt::white));
    p.drawText(r, Qt::AlignLeft | Qt::AlignTop, QString("%1\n%2\n%3").arg(start, end, width));
}

void BinaryMap::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
        emit ctrlModifier(false);
    else if(event->key() & Qt::Key_Shift)
        emit shiftModifier(false);

    QGLWidget::keyReleaseEvent(event);
}

void BinaryMap::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier)
        emit ctrlModifier(true);

    if(event->modifiers() & Qt::ShiftModifier)
        emit shiftModifier(true);

    if(event->modifiers() == Qt::NoModifier)
    {
        if(event->key() == Qt::Key_Plus)
            this->_step++;
        else if(event->key() == Qt::Key_Minus)
            this->_step--;

        if(this->_step < 1)
            this->_step = 1;
        else if(this->_step > this->_bytebuffer->length())
            this->_step = this->_bytebuffer->length();

        emit stepChanged(this->_step);
    }

    QGLWidget::keyPressEvent(event);
}

void BinaryMap::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        qint64 offset = this->calcOffset(event->pos());

        if(offset != -1)
        {
            emit gotoTriggered(offset);
            return;
        }
    }

    QGLWidget::mouseDoubleClickEvent(event);
}

void BinaryMap::mouseMoveEvent(QMouseEvent *event)
{
    qint64 offset = this->calcOffset(event->pos());

    if(offset != -1)
        emit offsetChanged(QString("%1h").arg(offset, 8, 16, QLatin1Char('0')).toUpper());

    QGLWidget::mouseMoveEvent(event);
}

void BinaryMap::wheelEvent(QWheelEvent* event)
{
    if(!this->_bytebuffer || (event->orientation() != Qt::Vertical))
        event->ignore();
    else
    {
        int numdegrees = event->delta() / 8;
        int numsteps = numdegrees / 15;

        if(event->modifiers() == Qt::NoModifier)
            this->setStart(this->_start - (numsteps * this->_step));

        if(event->modifiers() == Qt::ShiftModifier)
            this->setEnd(this->_end - (numsteps * this->_step));

        if(this->_displaymode == BinaryMap::ByteClass && (event->modifiers() == Qt::ControlModifier))
            this->setWidth(this->_width - (numsteps * this->_step));
    }

    QGLWidget::wheelEvent(event);
}

void BinaryMap::paintEvent(QPaintEvent *)
{
    if(this->_start == -1 || this->_end == -1 || this->_width == -1)
        return;

    QPainter p(this);

    if(!this->_bytebuffer)
    {
        this->drawNoDataAvailable(p);
        return;
    }

    if(this->_displaymode != BinaryMap::None)
    {
        switch(this->_displaymode)
        {
            case BinaryMap::ByteClass:
                this->renderByteClassMap(p);
                break;

            case BinaryMap::DotPlot:
                this->renderDotPlotMap(p);
                break;

            default:
                break;
        }

        this->drawInfo(p);
    }
}
