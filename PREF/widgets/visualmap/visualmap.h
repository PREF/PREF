#ifndef VISUALMAP_H
#define VISUALMAP_H

#include <QWidget>
#include "viewmodes/dotplotviewmode.h"
#include "viewmodes/pixelviewmode.h"

class VisualMap : public QWidget
{
    Q_OBJECT

    public:
        enum DisplayMode { DotPlot, BytesAsPixel };

    private:
        typedef QHash<VisualMap::DisplayMode, AbstractViewMode*> ViewMode;

    public:
        explicit VisualMap(QWidget *parent = 0);

    public slots:
        void setWidth(qint64 w);
        void setDisplayMode(VisualMap::DisplayMode mode);
        void initialize(QHexEdit *hexedit);

    private:
        qint64 calcOffset(const QPoint &cursorpos);
        void populateViewModes();
        void drawNoDataAvailable(QPainter& p);

    protected:
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void mouseMoveEvent(QMouseEvent* event);
        virtual void wheelEvent(QWheelEvent* event);
        virtual void paintEvent(QPaintEvent*);

    signals:
        void offsetChanged(qint64 offset);
        void gotoTriggered(qint64);

    private:
        static const QString NO_DATA_AVAILABLE;
        ViewMode _viewmodes;
        DisplayMode _viewmode;
        QHexEdit* _hexedit;
        qint64 _step;
        qint64 _width;

    friend class BinaryViewPage;
    friend class BinaryViewDialog;
};

#endif // BINARYMAP_H
