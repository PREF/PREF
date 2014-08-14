#ifndef DISASSEMBLERWIDGETPRIVATE_H
#define DISASSEMBLERWIDGETPRIVATE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/disassembler/listingobject.h"
#include "views/disassemblerview/disassemblerlisting.h"
#include "disassemblerhighlighter.h"

using namespace PrefSDK;

class DisassemblerWidgetPrivate: public QWidget
{
    Q_OBJECT

    public:
        class ListingItem
        {
            public:
                enum Type { Undefined, Listing, Reference };

            public:
                ListingItem(): _itemtype(ListingItem::Undefined), _listingobj(nullptr) { }
                ListingItem(ListingObject* listingobj): _itemtype(ListingItem::Listing), _listingobj(listingobj) { }
                ListingItem(const DisassemblerListing::ReferenceSet& references): _itemtype(ListingItem::Reference), _listingobj(nullptr), _references(references) { }
                const DisassemblerListing::ReferenceSet& references() const { return this->_references; }
                ListingObject* listingObject() const { return this->_listingobj; }
                ListingItem::Type itemType() const { return this->_itemtype; }
                bool isValid() const { return this->_itemtype != ListingItem::Undefined; }

            public:
                static const ListingItem& invalid() { return ListingItem::_invalidobj; }

            private:
                static ListingItem _invalidobj;

            private:
                ListingItem::Type _itemtype;
                ListingObject* _listingobj;
                DisassemblerListing::ReferenceSet _references;
        };

    public:
        explicit DisassemblerWidgetPrivate(QScrollArea* scrollarea, QScrollBar* vscrollbar, QWidget *parent = 0);
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
        QString functionType(Function *f) const;
        QString displayReferences(const QString& prefix, const DisassemblerListing::ReferenceSet& references) const;
        QString emitSegment(Segment* segment);
        QString emitFunction(Function *func);
        QString emitInstruction(Instruction *instruction);
        QString emitReferences(QFontMetrics &fm, const DisassemblerListing::ReferenceSet& references, int &x);
        int drawAddress(QPainter &painter, QFontMetrics &fm, ListingObject* listingobj, int y);
        int calcAddressWidth(QFontMetrics &fm, Reference *reference);
        void elaborateListing();
        void gotoFirstEntryPoint();
        void drawLineBackground(QPainter& painter, qint64 idx, int y);
        void drawLine(QPainter& painter, QFontMetrics& fm, qint64 idx, int y);
        void adjust();
        void ensureVisible(int idx);

    private slots:
        void onVScrollBarValueChanged(int);

    protected:
        void wheelEvent(QWheelEvent* e);
        void paintEvent(QPaintEvent* pe);
        void resizeEvent(QResizeEvent*);
        void mousePressEvent(QMouseEvent* e);

    private:
        DisassemblerListing* _listing;
        QVector<ListingItem> _listingitems;
        QHash<ListingObject*, int> _listingindexes;
        QScrollArea* _scrollarea;
        QScrollBar* _vscrollbar;
        QColor _addressforecolor;
        QColor _sellinecolor;
        int _wheelscrolllines;
        int _charwidth;
        int _charheight;
        int _currentindex;
};

#endif // DISASSEMBLERWIDGET_H
