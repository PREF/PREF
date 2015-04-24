#ifndef BINARYNAVIGATOR_H
#define BINARYNAVIGATOR_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include <QDebug>
#include <map/binarymap.h>
#include <support/bytecolors.h>
#include "prefsdk/databuffer/qdatabuffer.h"
#include "qhexedit/qhexedit.h"

using namespace PrefLib::Map;
using namespace PrefLib::Support;

class BinaryNavigator : public QGLWidget
{
    Q_OBJECT

    public:
        explicit BinaryNavigator(QWidget *parent = 0);
        void setData(QHexEdit* hexedit);
        void displayDefault();
        void displayEntropy();

    public slots:
        void renderMap(int = 0);

    private:
        qint64 indexFromPoint(const QPoint &pt);
        void adjust();

    private slots:
        void updateSquare(qint64);

    protected:
        virtual void wheelEvent(QWheelEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void paintEvent(QPaintEvent*);

    private:
        static const qreal BYTES_PER_LINE;
        BinaryMap _binarymap;
        QHexEditData* _hexeditdata;
        QHexEdit* _hexedit;
        uint64_t _size;
        uint64_t _offset;
        uint64_t _endoffset;
};

#endif // BINARYNAVIGATOR_H
