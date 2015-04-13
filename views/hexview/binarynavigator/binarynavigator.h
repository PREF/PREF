#ifndef BINARYNAVIGATOR_H
#define BINARYNAVIGATOR_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include "qhexedit/qhexedit.h"
#include <support/bytecolors.h>

using namespace PrefLib::Support;

class BinaryNavigator : public QGLWidget
{
    Q_OBJECT

    private:
        enum DisplayMode { ByteClass = 0, Entropy = 1, Default = ByteClass };

    public:
        explicit BinaryNavigator(QWidget *parent = 0);
        void setData(QHexEdit* hexedit);
        void displayDefault();
        void displayEntropy();

    public slots:
        void renderMap(int = 0);
        void renderEntropy(QPainter& p, qint64 x, qint64 y, QRectF &cursorrect, QColor& cursorcolor);
        void renderByteClass(QPainter& p, qint64 x, qint64 y, QRectF& cursorrect, QColor& cursorcolor);

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
        BinaryNavigator::DisplayMode _displaymode;
        QHexEditData* _hexeditdata;
        QHexEdit* _hexedit;
        qint64 _size;
        qint64 _maxwidth;
        qint64 _maxheight;
        qint64 _offset;
};

#endif // BINARYNAVIGATOR_H
