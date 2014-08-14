#ifndef DISASSEMBLERWIDGET_H
#define DISASSEMBLERWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "views/disassemblerview/disassemblerlisting.h"
#include "disassemblerwidgetprivate.h"

using namespace PrefSDK;

class DisassemblerWidget : public QScrollArea
{
    Q_OBJECT

    public:
        typedef DisassemblerWidgetPrivate::ListingItem ListingItem;

    public:
        explicit DisassemblerWidget(QWidget *parent = 0);
        const ListingItem& selectedItem();
        void setCurrentIndex(int idx);
        void setListing(DisassemblerListing* listing);
        void setAddressForeColor(const QColor& c);
        void setSelectedLineColor(const QColor& c);
        void setWheelScrollLines(int c);
        void selectItem(ListingObject* listingobj);
        void gotoAddress(uint64_t address);
        int currentIndex() const;

    private:
        DisassemblerListing* _listing;
        DisassemblerWidgetPrivate* _disasmwidget_p;
        QScrollArea* _scrollarea;
        QScrollBar* _vscrollbar;
        QHBoxLayout* _hlayout;
};

#endif // DISASSEMBLERWIDGET_H
