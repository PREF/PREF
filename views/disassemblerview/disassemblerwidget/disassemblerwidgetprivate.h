#ifndef DISASSEMBLERWIDGETPRIVATE_H
#define DISASSEMBLERWIDGETPRIVATE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/disassembler/disassemblerdefinition.h"
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "prefsdk/disassembler/listingprinter.h"
#include "disassemblerhighlighter.h"

using namespace PrefSDK;

class DisassemblerWidgetPrivate: public QWidget
{
    Q_OBJECT

    public:
        explicit DisassemblerWidgetPrivate(QScrollArea* scrollarea, QScrollBar* vscrollbar, QWidget *parent = 0);
        qint64 currentIndex() const;
        void setCurrentIndex(qint64 idx, bool savehistory = true);
        Block* selectedBlock() const;
        void setDisassembler(DisassemblerDefinition* disassembler);
        void setAddressForeColor(const QColor& c);
        void setSelectedLineColor(const QColor& c);
        void setWheelScrollLines(int c);
        void jumpTo(Block* block);
        void jumpTo(const DataValue &address);
        void clearNavigationHistory();
        void back();
        void forward();
        void save(const QString& filename);

    private:
        Block* findBlock(qint64 idx);
        QString functionType(Function *f) const;
        QString emitSegment(Segment* segment);
        QString emitFunction(Function *func);
        QString emitLabel(Label* label);
        QString displayReferences(const QString &prefix, Block *block);
        QString emitLine(qint64 idx);
        qint64 visibleStart(QRect r = QRect()) const;
        qint64 visibleEnd(QRect r = QRect()) const;
        int drawAddress(QPainter &painter, QFontMetrics &fm, Block* block, int y);
        void drawLineBackground(QPainter& painter, qint64 idx, int y);
        void drawLine(QPainter& painter, QFontMetrics& fm, qint64 idx, int y);
        void drawInstruction(Instruction *instruction, QPainter &painter, const QFontMetrics &fm, int x, int y);
        void ensureVisible(qint64 idx);
        void adjust();
        void pushBack(qint64 idx);
        void pushForward(qint64 idx);
        qint64 popBack();
        qint64 popForward();

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
        void backAvailable(bool);
        void forwardAvailable(bool);

    private:
        QScrollArea* _scrollarea;
        QScrollBar* _vscrollbar;
        ListingPrinter* _printer;
        DisassemblerDefinition* _disassembler;
        DisassemblerListing* _listing;
        QColor _addressforecolor;
        QColor _sellinecolor;
        Block* _selectedblock;
        int _wheelscrolllines;
        int _charwidth;
        int _charheight;
        qint64 _selectedindex;
        bool _clicked;
        QStack<qint64> _backstack;
        QStack<qint64> _forwardstack;

    private:
        Segment* _currentsegment;
        Function* _currentfunction;
};

#endif // DISASSEMBLERWIDGET_H
