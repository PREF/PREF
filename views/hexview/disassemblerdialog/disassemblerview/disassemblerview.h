#ifndef DISASSEMBLERVIEW_H
#define DISASSEMBLERVIEW_H

#include "disassemblerviewprivate.h"

class DisassemblerView : public QFrame
{
    Q_OBJECT

    public:
        explicit DisassemblerView(QWidget *parent = 0);
        void setInstructionCount(quint64 instructioncount);
        void setData(QHexEditData* hexeditdata);
        void setLoader(const ProcessorLoader& dl);
        void gotoVA(quint64 va);
        void gotoEP();

    private:
        DisassemblerViewPrivate* _disasmview_p;
        QScrollArea* _scrollarea;
        QScrollBar* _vscrollbar;
        QHBoxLayout* _hlayout;
};

#endif // DISASSEMBLERVIEW_H
