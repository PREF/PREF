#include "crossreferencedelegate.h"

CrossReferenceDelegate::CrossReferenceDelegate(DisassemblerListing *listing, QObject *parent): QStyledItemDelegate(parent), _listing(listing)
{
}

void CrossReferenceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() != 2)
    {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    QStyleOptionViewItem options(option);
    this->initStyleOption(&options, index);

    options.text = QString();
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    QTextDocument document;
    Reference* r = reinterpret_cast<Reference*>(index.internalPointer());
    Block* b = this->_listing->findBlock(r->referencedAddress());

    if(!b)
    {
        document.setPlainText("<Invalid Reference>");

        painter->save();
            painter->translate(option.rect.left(), option.rect.top());
            document.drawContents(painter, QRect(0, 0, options.rect.width(), options.rect.height()));
        painter->restore();
        return;
    }

    if((b->blockType() != Block::SegmentBlock) && (b->blockType() != Block::ReferenceBlock))
    {
        if(b->blockType() == Block::InstructionBlock)
            document.setPlainText(this->_listing->formatInstruction(qobject_cast<Instruction*>(b)));
        else if(b->blockType() == Block::FunctionBlock)
        {
            SymbolTable* symboltable = this->_listing->symbolTable();
            document.setPlainText(QString("function %1()").arg(symboltable->get(b->startAddress())));
        }
    }

    DisassemblerHighlighter highlighter(&document, b);
    highlighter.rehighlight();  /* Apply Syntax Highlighting */

    painter->save();
        painter->translate(option.rect.left(), option.rect.top());
        document.drawContents(painter, QRect(0, 0, options.rect.width(), options.rect.height()));
    painter->restore();
}

QSize CrossReferenceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem options(option);
    this->initStyleOption(&options, index);

    QTextDocument document;
    document.setPlainText(options.text);
    document.setTextWidth(options.rect.width());

    return QSize(document.idealWidth(), document.size().height());
}
