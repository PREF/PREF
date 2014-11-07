#ifndef CROSSREFERENCEDELEGATE_H
#define CROSSREFERENCEDELEGATE_H

#include "prefsdk/disassembler/blocks/instruction.h"
#include "prefsdk/disassembler/listingprinter.h"
#include "prefsdk/disassembler/disassemblerdefinition.h"
#include "views/disassemblerview/disassemblerwidget/disassemblerhighlighter.h"
#include <QStyledItemDelegate>

using namespace PrefSDK;

class CrossReferenceDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit CrossReferenceDelegate(Block *block, DisassemblerDefinition* disassembler, DisassemblerListing* listing, QObject *parent = 0);

    public: /* Overriden Methods */
        virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    private:
        const QList<DataValue>& _sources;
        DisassemblerDefinition* _disassembler;
        DisassemblerListing* _listing;
        QFont _monospacefont;
};

#endif // CROSSREFERENCEDELEGATE_H
