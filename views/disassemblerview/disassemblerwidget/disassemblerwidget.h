#ifndef DISASSEMBLERWIDGET_H
#define DISASSEMBLERWIDGET_H

#include "disassemblerwidgetprivate.h"

class DisassemblerWidget : public QFrame
{
    Q_OBJECT

    public:
        explicit DisassemblerWidget(QWidget *parent = 0);
        void setInstructionCount(quint64 instructioncount);
        void setData(QHexEditData* hexeditdata);
        void setLoader(const ProcessorLoader& dl);
        void gotoVA(quint64 va);
        void gotoEP();

    private:
        DisassemblerWidgetPrivate* _disasmwidget_p;
        QScrollArea* _scrollarea;
        QScrollBar* _vscrollbar;
        QHBoxLayout* _hlayout;
};

#endif // DISASSEMBLERWIDGET_H
