#ifndef DISASSEMBLERWIDGETPRIVATE_H
#define DISASSEMBLERWIDGETPRIVATE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/sdkmanager.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/disassembler/processorloader.h"
#include "views/disassemblerview/disassemblerwidget/disassemblerdrawer.h"

using namespace PrefSDK;

class DisassemblerWidgetPrivate : public QWidget
{
    Q_OBJECT

    public:
        explicit DisassemblerWidgetPrivate(QScrollArea* scrollarea, QScrollBar* vscrollbar, QWidget *parent = 0);
        void setInstructionCount(quint64 instructioncount);
        void setData(QHexEditData* hexeditdata);
        void gotoVA(quint64 va);
        void gotoEP();
        void setLoader(const ProcessorLoader& dl);

    private:
        void adjust();
        qint64 verticalSliderPosition64();
        bool drawLine(DisassemblerDrawer *drawer, lua_Integer i);

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
        int64_t _instructioncount;
        int _labelwidth;
        int _charwidth;
        int _charheight;
        int _hexdumpwidth;
        bool _incall;
};

#endif // DISASSEMBLERWIDGETPRIVATE_H
