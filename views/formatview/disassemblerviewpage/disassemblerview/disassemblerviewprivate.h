#ifndef DISASSEMBLERVIEWPRIVATE_H
#define DISASSEMBLERVIEWPRIVATE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/sdkmanager.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/disassembler/processorloader.h"
#include "views/formatview/disassemblerviewpage/disassemblerview/disassemblerviewdrawer.h"

using namespace PrefSDK;

class DisassemblerViewPrivate : public QWidget
{
    Q_OBJECT

    public:
        explicit DisassemblerViewPrivate(QScrollArea* scrollarea, QScrollBar* vscrollbar, QWidget *parent = 0);
        void setInstructionCount(quint64 instructioncount);
        void setData(QHexEditData* hexeditdata);
        void gotoVA(quint64 va);
        void gotoEP();
        void setLoader(const ProcessorLoader& dl);

    private:
        void adjust();
        qint64 verticalSliderPosition64();
        //void setCharColor(InstructionItem *ii, const QChar& ch, QPainter& painter);
        //void setInstructionColor(InstructionItem* ii, QPainter& painter);
        bool drawLine(DisassemblerViewDrawer *drawer, lua_Integer i);
        //int drawAddress(const QString &segmentname, QPainter &painter, QFontMetrics &fm, ListingItem *li, int y);
        //void drawHexDump(InstructionItem *ii, QPainter &painter, QFontMetrics &fm, int x, int y);
        //void drawLabel(LabelItem* li, QPainter &painter, QFontMetrics &fm, int x, int y);
        //void drawComment(const QString& s, QPainter &painter, QFontMetrics &fm, int x, int y);
        //void drawInstruction(InstructionItem *ii, QPainter& painter, QFontMetrics& fm, int x, int y);
        //void drawReference(const ReferenceTable::Reference& ref, lua_Integer ignoreaddress, QPainter &painter, QFontMetrics &fm, int x, int y);

    private slots:
        void vScrollBarValueChanged(int);

    protected:
        virtual void paintEvent(QPaintEvent* e);
        virtual void resizeEvent(QResizeEvent*);
        virtual void wheelEvent(QWheelEvent* event);
        virtual void mousePressEvent(QMouseEvent*);
        virtual void keyPressEvent(QKeyEvent* e);

    private:
        ProcessorLoader _loader;
        FormatList::LoadedFormat _loadedformat;
        QHexEditData* _hexeditdata;
        QScrollArea* _scrollArea;
        QScrollBar* _vscrollbar;
        QColor _nofeaturecolor;
        QColor _callcolor;
        QColor _stopcolor;
        QColor _jumpcolor;
        QColor _digitcolor;
        QColor _symbolcolor;
        uint64_t _instructioncount;
        int _labelwidth;
        int _charwidth;
        int _charheight;
        int _hexdumpwidth;
        bool _incall;
};

#endif // DISASSEMBLERVIEWPRIVATE_H
