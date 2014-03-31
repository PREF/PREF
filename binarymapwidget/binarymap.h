#ifndef BINARYMAP_H
#define BINARYMAP_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/math.h"
#include "prefsdk/bytecolors.h"
#include "prefsdk/qlua.h"

using namespace PrefSDK;

class BinaryMap : public QGLWidget
{
    Q_OBJECT

    public:
        enum DisplayMode { None, ByteClass, DotPlot };

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
        QByteArray data();
        qint64 calcOffset(const QPoint &cursorpos);
        qint64 calcDotPlotOffset(const QPoint& cursorpos);
        qint64 calcByteClassOffset(const QPoint& cursorpos);
        void renderByteClassMap(QPainter &p);
        void renderDotPlotMap(QPainter &p);
        void drawNoDataAvailable(QPainter& p);
        void drawInfo(QPainter& p);

    protected:
        virtual void keyReleaseEvent(QKeyEvent* event);
        virtual void keyPressEvent(QKeyEvent* event);
        virtual void mouseDoubleClickEvent(QMouseEvent* event);
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
        QHash<lua_Integer, lua_Integer> _drawedoffset;
        DisplayMode _displaymode;
        QHexEditData* _hexeditdata;
        QByteArray _bits;
        qint64 _step;
        qint64 _start;
        qint64 _end;
        qint64 _width;

    friend class BinaryViewPage;
};

#endif // BINARYMAP_H
