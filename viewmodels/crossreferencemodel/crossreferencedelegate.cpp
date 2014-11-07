#include "crossreferencedelegate.h"

CrossReferenceDelegate::CrossReferenceDelegate(Block* block, DisassemblerDefinition* disassembler, DisassemblerListing *listing, QObject *parent): QStyledItemDelegate(parent), _sources(block->sources()), _disassembler(disassembler), _listing(listing)
{
    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);
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
    options.font = this->_monospacefont;
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    QTextDocument document;
    DataValue sourceaddress = this->_sources[index.row()];
    Block* b = this->_listing->findBlock(sourceaddress);

    if(b->blockType() == Block::InstructionBlock)
    {
        ListingPrinter printer(this->_disassembler->addressType());
        this->_disassembler->callOutput(&printer, qobject_cast<Instruction*>(b));
        printer.draw(painter, options.fontMetrics, options.rect.left(), options.rect.top());
        return;
    }

    if(b->blockType() == Block::FunctionBlock)
    {
        SymbolTable* symboltable = this->_listing->symbolTable();
        document.setPlainText(QString("function %1()").arg(symboltable->name(b->startAddress())));
    }

    DisassemblerHighlighter highlighter(&document, b);
    highlighter.rehighlight();  /* Apply Syntax Highlighting */

    painter->save();
        painter->translate(options.rect.left(), options.rect.top());
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
