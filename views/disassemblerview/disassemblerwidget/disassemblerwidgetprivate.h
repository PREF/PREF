#ifndef DISASSEMBLERWIDGETPRIVATE_H
#define DISASSEMBLERWIDGETPRIVATE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "disassemblerhighlighter.h"

using namespace PrefSDK;

class DisassemblerWidgetPrivate: public QWidget
{
    Q_OBJECT

    public:
        explicit DisassemblerWidgetPrivate(QScrollArea* scrollarea, QScrollBar* vscrollbar, QWidget *parent = 0);
        qint64 currentIndex() const;
        void setCurrentIndex(qint64 idx);
        Block* selectedBlock() const;
        void setListing(DisassemblerListing* listing);
        void setAddressForeColor(const QColor& c);
        void setSelectedLineColor(const QColor& c);
        void setWheelScrollLines(int c);
        void jumpTo(Block* block);
        void jumpTo(const DataValue &address);

    private:
        Block* findBlock(qint64 idx);
        QString functionType(Function *f) const;
        QString displayReferences(const QString& prefix, const ReferenceSet *referenceset) const;
        QString emitSegment(Segment* segment);
        QString emitFunction(Function *func);
        QString emitInstruction(Instruction *instruction);
        QString emitReference(ReferenceSet* referenceset);
        qint64 visibleStart(QRect r = QRect()) const;
        qint64 visibleEnd(QRect r = QRect()) const;
        int drawAddress(QPainter &painter, QFontMetrics &fm, Block* block, int y);
        void drawLineBackground(QPainter& painter, qint64 idx, int y);
        void drawLine(QPainter& painter, QFontMetrics& fm, qint64 idx, int y);
        void ensureVisible(qint64 idx);
        void adjust();

    protected:
        void keyPressEvent(QKeyEvent* e);
        void wheelEvent(QWheelEvent* e);
        void paintEvent(QPaintEvent* pe);
        void resizeEvent(QResizeEvent*);
        void mousePressEvent(QMouseEvent* e);
        void mouseDoubleClickEvent(QMouseEvent* e);

    private slots:
        void unlockClick();
        void updateLines(int);

    signals:
        void jumpToRequested();
        void crossReferenceRequested(Block* block);

    private:
        QScrollArea* _scrollarea;
        QScrollBar* _vscrollbar;
        DisassemblerListing* _listing;
        QColor _addressforecolor;
        QColor _sellinecolor;
        Block* _selectedblock;
        int _wheelscrolllines;
        int _charwidth;
        int _charheight;
        qint64 _selectedindex;
        bool _clicked;

    private:
        Segment* _currentsegment;
        Function* _currentfunction;
};

#endif // DISASSEMBLERWIDGET_H
