#ifndef CROSSREFERENCEDELEGATE_H
#define CROSSREFERENCEDELEGATE_H

#include "prefsdk/disassembler/blocks/instruction.h"
#include "prefsdk/disassembler/references/reference.h"
#include "views/disassemblerview/disassemblerwidget/disassemblerhighlighter.h"
#include <QStyledItemDelegate>

using namespace PrefSDK;

class CrossReferenceDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit CrossReferenceDelegate(DisassemblerListing* listing, QObject *parent = 0);

    public: /* Overriden Methods */
        virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    private:
        DisassemblerListing* _listing;
};

#endif // CROSSREFERENCEDELEGATE_H
