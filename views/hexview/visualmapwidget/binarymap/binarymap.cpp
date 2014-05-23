#include "binarymap.h"

const QString BinaryMap::NO_DATA_AVAILABLE = "No Data Available";

BinaryMap::BinaryMap(QWidget *parent): QGLWidget(parent), _viewmode(BinaryMap::DotPlot), _hexedit(nullptr), _step(0), _width(256)
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

void BinaryMap::setWidth(qint64 w)
{
    if(w < 0)
        w = 0;

    this->_width = w;

    if(this->_hexedit)
        this->update();
}

void BinaryMap::setDisplayMode(BinaryMap::DisplayMode mode)
{
    this->_viewmode = mode;

    if(this->_hexedit)
        this->update();
}

void BinaryMap::setData(QHexEdit* hexedit)
{
    this->_hexedit = hexedit;
    this->populateViewModes();

    connect(this->_hexedit, SIGNAL(verticalScrollBarValueChanged(int)), this, SLOT(updateMap(int)));
    connect(this->_hexedit, SIGNAL(selectionChanged(qint64)), this, SLOT(updateMap(qint64)));
}

qint64 BinaryMap::calcOffset(const QPoint &cursorpos)
{
    return this->_viewmodes[this->_viewmode]->offset(QVector2D(cursorpos));
}

void BinaryMap::populateViewModes()
{
    this->_viewmodes.clear();
    this->_viewmodes[BinaryMap::DotPlot] = new DotPlotViewMode(this->_hexedit, this);
    this->_viewmodes[BinaryMap::BytesAsPixel] = new PixelViewMode(this->_hexedit, this);
}

void BinaryMap::drawNoDataAvailable(QPainter &p)
{
    QFontMetrics fm = this->fontMetrics();

    p.setPen(QColor(Qt::white));
    p.drawText(10, 10, fm.width(BinaryMap::NO_DATA_AVAILABLE), fm.height(), Qt::AlignLeft | Qt::AlignTop, BinaryMap::NO_DATA_AVAILABLE);
}

void BinaryMap::updateMap(int)
{
    this->update();
}

void BinaryMap::updateMap(qint64)
{
    this->update();
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
        emit offsetChanged(offset);

    QGLWidget::mouseMoveEvent(event);
}

void BinaryMap::wheelEvent(QWheelEvent *event)
{
    this->_hexedit->scroll(event);
    QGLWidget::wheelEvent(event);
}

void BinaryMap::paintEvent(QPaintEvent *)
{
    if(!this->_viewmodes.contains(this->_viewmode) || this->_width == -1)
        return;

    QPainter p(this);

    if(!this->_hexedit)
    {
        this->drawNoDataAvailable(p);
        return;
    }

    this->_viewmodes[this->_viewmode]->render(p, this->_width);
}
