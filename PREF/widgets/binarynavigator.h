#ifndef BINARYNAVIGATOR_H
#define BINARYNAVIGATOR_H

#include <QWidget>
#include <map/binarymap.h>
#include <support/bytecolors.h>
#include <qhexedit/qhexedit.h>
#include "../platform/loadeddata.h"

using namespace PrefLib::Map;
using namespace PrefLib::Support;

class BinaryNavigator : public QWidget
{
    Q_OBJECT

    public:
        explicit BinaryNavigator(QWidget *parent = 0);
        void initialize(QHexEdit* hexedit, LoadedData* loadeddata);

    public slots:
        void displayDefault();
        void switchView();

    private:
        integer_t offsetFromPoint(const QPoint &pt);
        void adjust();

    private slots:
        void updateSquare();
        void renderMap();

    protected:
        virtual void wheelEvent(QWheelEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void paintEvent(QPaintEvent*);
        virtual void resizeEvent(QResizeEvent *event);

    private:
        static const uint64_t BYTES_PER_LINE;
        BinaryMap _binarymap;
        QHexEdit* _hexedit;
        LoadedData* _loadeddata;
        uint64_t _squaresize;
        integer_t _offset;
        integer_t _endoffset;
};

#endif // BINARYNAVIGATOR_H
