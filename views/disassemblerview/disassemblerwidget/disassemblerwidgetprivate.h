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

    private:
        class ListingItem
        {
            public:
                enum Type { Listing, Reference };

            public:
                ListingItem(ListingObject* listingobj): _itemtype(ListingItem::Listing), _listingobj(listingobj) { }
                ListingItem(const DisassemblerListing::ReferenceSet& references): _itemtype(ListingTypes::Reference), _listingobj(nullptr), _references(references) { }
                const DisassemblerListing::ReferenceSet& references() const { return this->_references; }
                ListingObject* listingObject() const { return this->_listingobj; }
                ListingItem::Type itemType() const { return this->_itemtype; }

            private:
                ListingItem::Type _itemtype;
                ListingObject* _listingobj;
                DisassemblerListing::ReferenceSet _references;
        };

    public:
        explicit DisassemblerWidgetPrivate(QScrollArea* scrollarea, QScrollBar* vscrollbar, QWidget *parent = 0);
        void setCurrentIndex(int idx);
        void setListing(DisassemblerListing* listing);
        void setAddressForeColor(const QColor& c);
        void setSelectedLineColor(const QColor& c);
        void setWheelScrollLines(int c);
        void gotoFunction(Function* func);
        int currentIndex() const;

    private:
        ListingObject* findListingObject(int idx);
        QString functionType(Function *f) const;
        QString displayReferences(const QString& prefix, const DisassemblerListing::ReferenceSet& references) const;
        QString emitSegment(Segment* segment);
        QString emitFunction(Function *func);
        QString emitInstruction(Instruction *instruction, int &ident);
        int drawAddress(QPainter &painter, QFontMetrics &fm, ListingObject* listingobj, int y);
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
        QVector<ListingObject*> _listingitems;
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
