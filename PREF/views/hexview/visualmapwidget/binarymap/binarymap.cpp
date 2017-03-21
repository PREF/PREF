#include "binarymap.h"

const QString BinaryMapWidget::NO_DATA_AVAILABLE = "No Data Available";

BinaryMapWidget::BinaryMapWidget(QWidget *parent): QGLWidget(parent), _viewmode(BinaryMapWidget::DotPlot), _hexedit(nullptr), _step(0), _width(256)
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

void BinaryMapWidget::setWidth(qint64 w)
{
    if(w < 0)
        w = 0;

    this->_width = w;

    if(this->_hexedit)
        this->update();
}

void BinaryMapWidget::setDisplayMode(BinaryMapWidget::DisplayMode mode)
{
    this->_viewmode = mode;

    if(this->_hexedit)
        this->update();
}

void BinaryMapWidget::setData(QHexEdit* hexedit)
{
    this->_hexedit = hexedit;
    this->populateViewModes();

    connect(this->_hexedit, SIGNAL(verticalScrollBarValueChanged(int)), this, SLOT(updateMap(int)));
    connect(this->_hexedit, SIGNAL(selectionChanged(qint64)), this, SLOT(updateMap(qint64)));
}

qint64 BinaryMapWidget::calcOffset(const QPoint &cursorpos)
{
    return this->_viewmodes[this->_viewmode]->offset(QVector2D(cursorpos));
}

void BinaryMapWidget::populateViewModes()
{
    this->_viewmodes.clear();
    this->_viewmodes[BinaryMapWidget::DotPlot] = new DotPlotViewMode(this->_hexedit, this);
    this->_viewmodes[BinaryMapWidget::BytesAsPixel] = new PixelViewMode(this->_hexedit, this);
}

void BinaryMapWidget::drawNoDataAvailable(QPainter &p)
{
    QFontMetrics fm = this->fontMetrics();

    p.setPen(QColor(Qt::white));
    p.drawText(10, 10, fm.width(BinaryMapWidget::NO_DATA_AVAILABLE), fm.height(), Qt::AlignLeft | Qt::AlignTop, BinaryMapWidget::NO_DATA_AVAILABLE);
}

void BinaryMapWidget::updateMap(int)
{
    this->update();
}

void BinaryMapWidget::updateMap(qint64)
{
    this->update();
}

void BinaryMapWidget::mousePressEvent(QMouseEvent *event)
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

void BinaryMapWidget::mouseMoveEvent(QMouseEvent *event)
{
    qint64 offset = this->calcOffset(event->pos());

    if(offset != -1)
        emit offsetChanged(offset);

    QGLWidget::mouseMoveEvent(event);
}

void BinaryMapWidget::wheelEvent(QWheelEvent *event)
{
    this->_hexedit->scroll(event);
    QGLWidget::wheelEvent(event);
}

void BinaryMapWidget::paintEvent(QPaintEvent *)
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
