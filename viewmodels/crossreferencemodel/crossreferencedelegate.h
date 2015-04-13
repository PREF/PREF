#ifndef CROSSREFERENCEDELEGATE_H
#define CROSSREFERENCEDELEGATE_H

#include <QStyledItemDelegate>
#include <disassembler/disassemblerdefinition.h>
#include "views/disassemblerview/disassemblerwidget/disassemblerhighlighter.h"

using namespace PrefLib::Disassembler;

class CrossReferenceDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit CrossReferenceDelegate(Block *block, DisassemblerDefinition* disassembler, QObject *parent = 0);

    public: /* Overriden Methods */
        virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    private:
        DisassemblerDefinition* _disassembler;
        QFont _monospacefont;
};

#endif // CROSSREFERENCEDELEGATE_H
