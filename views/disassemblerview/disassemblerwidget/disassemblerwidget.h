#ifndef DISASSEMBLERWIDGET_H
#define DISASSEMBLERWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "disassemblerwidgetprivate.h"

using namespace PrefSDK;

class DisassemblerWidget : public QFrame
{
    Q_OBJECT

    public:
        explicit DisassemblerWidget(QWidget *parent = 0);
        void setCurrentIndex(int idx);
        void setListing(DisassemblerListing* listing);
        void setAddressForeColor(const QColor& c);
        void setSelectedLineColor(const QColor& c);
        void setWheelScrollLines(int c);
        void jumpTo(Block* block);
        void jumpTo(const DataValue &address);
        qint64 currentIndex() const;
        Block* selectedBlock() const;

    signals:
        void jumpToRequested();
        void crossReferenceRequested(Block* block);

    private:
        DisassemblerWidgetPrivate* _disasmwidget_p;
        QScrollArea* _scrollarea;
        QScrollBar* _vscrollbar;
        QHBoxLayout* _hlayout;
};

#endif // DISASSEMBLERWIDGET_H
