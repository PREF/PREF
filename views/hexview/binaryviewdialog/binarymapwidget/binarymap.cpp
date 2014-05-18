#include "binarymap.h"

const QString BinaryMap::NO_DATA_AVAILABLE = "No Data Available";

BinaryMap::BinaryMap(QWidget *parent): QGLWidget(parent), _viewmode(BinaryMap::DotPlot), _hexeditdata(nullptr), _step(0), _start(-1), _end(-1), _width(256)
{
    QFont f("Monospace", qApp->font().pointSize());
    f.setStyleHint(QFont::TypeWriter);

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

    if(this->_hexeditdata)
        this->update();
}

void BinaryMap::setEnd(int end)
{
    if((this->_start != -1) && (end < this->_start))
        end =  this->_start + 1;
    else if(this->_hexeditdata && end >= this->_hexeditdata->length())
        end = this->_hexeditdata->length();

    this->_end = end;

    if(this->_hexeditdata)
        this->update();
}

void BinaryMap::setWidth(int w)
{
    if(w < 0)
        w = 0;

    this->_width = w;

    if(this->_hexeditdata)
        this->update();
}

void BinaryMap::setStep(qint64 step)
{
    this->_step = step;
}

void BinaryMap::setDisplayMode(BinaryMap::DisplayMode mode)
{
    this->_viewmode = mode;

    if(this->_hexeditdata)
        this->update();
}

void BinaryMap::setData(QHexEditData *hexeditdata)
{
    this->_hexeditdata = hexeditdata;
    this->populateViewModes();
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

qint64 BinaryMap::calcOffset(const QPoint &cursorpos)
{
    return this->_viewmodes[this->_viewmode]->offset(QVector2D(cursorpos));
}

void BinaryMap::populateViewModes()
{
    this->_viewmodes.clear();
    this->_viewmodes[BinaryMap::DotPlot] = new DotPlotViewMode(this->_hexeditdata, this);
    this->_viewmodes[BinaryMap::BytesAsPixel] = new PixelViewMode(this->_hexeditdata, this);
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
        else if(this->_step > this->_hexeditdata->length())
            this->_step = this->_hexeditdata->length();

        emit stepChanged(this->_step);
    }

    QGLWidget::keyPressEvent(event);
}

void BinaryMap::mousePressEvent(QMouseEvent *event)
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

    QGLWidget::mousePressEvent(event);
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
    if(!this->_hexeditdata || (event->orientation() != Qt::Vertical))
        event->ignore();
    else
    {
        int numdegrees = event->delta() / 8;
        int numsteps = numdegrees / 15;

        if(event->modifiers() == Qt::NoModifier)
            this->setStart(this->_start - (numsteps * this->_step));

        if(event->modifiers() == Qt::ShiftModifier)
            this->setEnd(this->_end - (numsteps * this->_step));

        if(this->_viewmode == BinaryMap::BytesAsPixel && (event->modifiers() == Qt::ControlModifier))
        {
            qint64 neww = this->_width - (numsteps * this->_step);

            if(neww <= 0)
                neww = 1;

            this->setWidth(neww);
        }
    }

    QGLWidget::wheelEvent(event);
}

void BinaryMap::paintEvent(QPaintEvent *)
{
    if(!this->_viewmodes.contains(this->_viewmode) || this->_start == -1 || this->_end == -1 || this->_width == -1)
        return;

    QPainter p(this);

    if(!this->_hexeditdata)
    {
        this->drawNoDataAvailable(p);
        return;
    }

    this->_viewmodes[this->_viewmode]->render(p, this->_start, this->_end, this->_width);
    this->drawInfo(p);
}
