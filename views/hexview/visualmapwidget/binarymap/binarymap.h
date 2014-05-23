#ifndef BINARYMAP_H
#define BINARYMAP_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include <lua.hpp>
#include "qhexedit/qhexedit.h"
#include "qhexedit/qhexeditdatareader.h"
#include "prefsdk/math.h"
#include "prefsdk/bytecolors.h"
#include "viewmodes/abstractviewmode.h"
#include "viewmodes/dotplotviewmode.h"
#include "viewmodes/pixelviewmode.h"

using namespace PrefSDK;

class BinaryMap : public QGLWidget
{
    Q_OBJECT

    public:
        enum DisplayMode { DotPlot, BytesAsPixel };

    private:
        typedef QHash<BinaryMap::DisplayMode, AbstractViewMode*> ViewMode;

    public:
        explicit BinaryMap(QWidget *parent = 0);

    public slots:
        void setWidth(qint64 w);
        void setDisplayMode(BinaryMap::DisplayMode mode);
        void setData(QHexEdit *hexedit);

    private:
        qint64 calcOffset(const QPoint &cursorpos);
        void populateViewModes();
        void drawNoDataAvailable(QPainter& p);

    private slots:
        void updateMap(int);
        void updateMap(qint64);

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
