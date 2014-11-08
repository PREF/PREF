#ifndef DISASSEMBLERWIDGET_H
#define DISASSEMBLERWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/disassembler/disassemblerdefinition.h"
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "disassemblerwidgetprivate.h"

using namespace PrefSDK;

class DisassemblerWidget : public QFrame
{
    Q_OBJECT

    public:
        explicit DisassemblerWidget(QWidget *parent = 0);
        void setDisassembler(DisassemblerDefinition* disassembler);
        void setListing(DisassemblerListing* listing);
        void setMemoryBuffer(MemoryBuffer* memorybuffer);
        void setAddressForeColor(const QColor& c);
        void setSelectedLineColor(const QColor& c);
        void setWheelScrollLines(int c);
        qint64 currentIndex() const;
        Block* selectedBlock() const;

    public slots:
        void setCurrentIndex(int idx);
        void jumpTo(Block* block);
        void jumpTo(const PrefSDK::DataValue &address);
        void save(const QString& filename);
        void clearNavigationHistory();
        void back();
        void forward();

    signals:
        void jumpToRequested();
        void crossReferenceRequested(Block* block);
        void backAvailable(bool);
        void forwardAvailable(bool);

    private:
        DisassemblerWidgetPrivate* _disasmwidget_p;
        QScrollArea* _scrollarea;
        QScrollBar* _vscrollbar;
        QHBoxLayout* _hlayout;
};

#endif // DISASSEMBLERWIDGET_H
