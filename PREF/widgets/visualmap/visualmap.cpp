#include "visualmap.h"
#include <QGuiApplication>
#include <QMouseEvent>
#include <QPainter>

const QString VisualMap::NO_DATA_AVAILABLE = "No Data Available";

VisualMap::VisualMap(QWidget *parent): QWidget(parent), _viewmode(VisualMap::DotPlot), _hexedit(NULL), _step(0), _width(256)
{
    QFont f("Monospace", qApp->font().pointSize());
    f.setStyleHint(QFont::TypeWriter);

    this->setMouseTracking(true);
    this->setAutoFillBackground(true);
    this->setCursor(QCursor(Qt::CrossCursor));
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFont(f);

    QPalette p = this->palette();
    p.setColor(QPalette::Background, QColor(Qt::black));

    this->setPalette(p);
}

void VisualMap::setWidth(qint64 w)
{
    if(w < 0)
        w = 0;

    this->_width = w;

    if(this->_hexedit)
        this->update();
}

void VisualMap::setDisplayMode(VisualMap::DisplayMode mode)
{
    this->_viewmode = mode;

    if(this->_hexedit)
        this->update();
}

void VisualMap::initialize(QHexEdit* hexedit)
{
    this->_hexedit = hexedit;
    this->populateViewModes();

    connect(this->_hexedit, &QHexEdit::verticalScroll, [this](integer_t) { this->update(); });
    connect(this->_hexedit->document()->cursor(), &QHexCursor::selectionChanged, [this]() { this->update(); });
}

qint64 VisualMap::calcOffset(const QPoint &cursorpos)
{
    return this->_viewmodes[this->_viewmode]->offset(QPoint(cursorpos));
}

void VisualMap::populateViewModes()
{
    this->_viewmodes.clear();
    this->_viewmodes[VisualMap::DotPlot] = new DotPlotViewMode(this->_hexedit, this);
    this->_viewmodes[VisualMap::BytesAsPixel] = new PixelViewMode(this->_hexedit, this);
}

void VisualMap::drawNoDataAvailable(QPainter &p)
{
    QFontMetrics fm = this->fontMetrics();

    p.setPen(QColor(Qt::white));
    p.drawText(10, 10, fm.width(VisualMap::NO_DATA_AVAILABLE), fm.height(), Qt::AlignLeft | Qt::AlignTop, VisualMap::NO_DATA_AVAILABLE);
}

void VisualMap::mousePressEvent(QMouseEvent *event)
{
    if(this->_hexedit && (event->buttons() == Qt::LeftButton))
    {
        qint64 offset = this->calcOffset(event->pos());

        if(offset != -1)
        {
            emit gotoTriggered(offset);
            return;
        }
    }

    QWidget::mousePressEvent(event);
}

void VisualMap::mouseMoveEvent(QMouseEvent *event)
{
    if(this->_hexedit)
    {
        qint64 offset = this->calcOffset(event->pos());

        if(offset != -1)
            emit offsetChanged(offset);
    }

    QWidget::mouseMoveEvent(event);
}

void VisualMap::wheelEvent(QWheelEvent *event)
{
    if(this->_hexedit)
        this->_hexedit->scroll(event);

    QWidget::wheelEvent(event);
}

void VisualMap::paintEvent(QPaintEvent *)
{
    if(!this->_viewmodes.contains(this->_viewmode) || this->_width == -1)
        return;

    QPainter p(this);

    if(!this->_hexedit)
    {
        this->drawNoDataAvailable(p);
        return;
    }

    this->_viewmodes[this->_viewmode]->render(&p, this->_width);
}
