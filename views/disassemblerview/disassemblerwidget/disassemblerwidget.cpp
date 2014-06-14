#include "disassemblerwidget.h"

DisassemblerWidget::DisassemblerWidget(QWidget *parent): QPlainTextEdit(parent), _listing(nullptr)
{    
    this->_highlighter = new DisassemblerHighlighter(this->document());

    QFont f("Monospace", qApp->font().pointSize());
    f.setStyleHint(QFont::TypeWriter);

    this->setFont(f);
    this->setReadOnly(true);
    this->setUndoRedoEnabled(false);
    this->highlightCurrentLine();

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
}

void DisassemblerWidget::setListing(DisassemblerListing *listing)
{
    this->_listing = listing;

    this->displayListing();
}

void DisassemblerWidget::gotoFunction(Function *func)
{
    QTextDocument* document = this->document();
    QTextCursor cursor(document->findBlockByLineNumber(this->_functionlines[func]));
    this->setTextCursor(cursor);
}

void DisassemblerWidget::displayListing()
{
    int linecount = 0;
    QTextCursor cursor = this->textCursor();

    cursor.beginEditBlock();

    for(int i = 0; i < this->_listing->segmentsCount(); i++)
    {
        Segment* segment = this->_listing->segment(i);
        SegmentBlock sb(this->_listing, segment, cursor);

        this->_segmentlines[segment] = linecount;
        linecount += sb.print();

        for(int j = 0; j < segment->functionsCount(); j++)
        {
            Function* func = segment->function(j);
            FunctionBlock fb(this->_listing, func, cursor);

            this->_functionlines[func] = linecount;
            linecount += fb.print();

            for(int k = 0; k < func->instructionsCount(); k++)
            {
                Instruction* instr = func->instruction(k);
                InstructionBlock ib(this->_listing, instr, cursor);

                this->_instructionlines[instr] = linecount;
                linecount += ib.print();
            }
        }
    }

    cursor.endEditBlock();
}

void DisassemblerWidget::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraselections;
    QTextEdit::ExtraSelection selection;

    selection.format.setBackground(QColor(Qt::yellow).lighter(160));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = this->textCursor();
    selection.cursor.clearSelection();
    extraselections.append(selection);

    this->setExtraSelections(extraselections);
}
