#ifndef BINARYMAP_H
#define BINARYMAP_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include <lua.hpp>
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
        enum DisplayMode { BytesAsPixel, DotPlot };

    private:
        typedef QHash<BinaryMap::DisplayMode, AbstractViewMode*> ViewMode;

    public:
        explicit BinaryMap(QWidget *parent = 0);

    public slots:
        void setStart(int start);
        void setEnd(int end);
        void setWidth(int w);
        void setStep(qint64 step);
        void setDisplayMode(BinaryMap::DisplayMode mode);
        void setData(QHexEditData* hexeditdata);

    private:
        qint64 start();
        qint64 end();
        qint64 preferredHeight();
        qint64 calcOffset(const QPoint &cursorpos);
        void populateViewModes();
        void drawNoDataAvailable(QPainter& p);
        void drawInfo(QPainter& p);

    protected:
        virtual void keyReleaseEvent(QKeyEvent* event);
        virtual void keyPressEvent(QKeyEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void mouseMoveEvent(QMouseEvent* event);
        virtual void wheelEvent(QWheelEvent* event);
        virtual void paintEvent(QPaintEvent*);

    signals:
        void ctrlModifier(bool pressed);
        void shiftModifier(bool pressed);
        void stepChanged(qint64 step);
        void offsetChanged(QString offsetstring);
        void gotoTriggered(qint64);

    private:
        static const QString NO_DATA_AVAILABLE;
        ViewMode _viewmodes;
        DisplayMode _viewmode;
        QHexEditData* _hexeditdata;
        QByteArray _bits;
        qint64 _step;
        qint64 _start;
        qint64 _end;
        qint64 _width;

    friend class BinaryViewPage;
    friend class BinaryViewDialog;
};

#endif // BINARYMAP_H
