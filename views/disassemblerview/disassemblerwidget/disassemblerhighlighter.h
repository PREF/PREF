#ifndef DISASSEMBLERHIGHLIGHTER_H
#define DISASSEMBLERHIGHLIGHTER_H

#include <QtCore>
#include <QtGui>
#include <disassembler/blocks/block.h>

using namespace PrefLib::Disassembler;

class DisassemblerHighlighter: public QSyntaxHighlighter
{
    Q_OBJECT

    public:
        explicit DisassemblerHighlighter(QTextDocument *parent, Block* block);

    private:
        void generateHighlighters();
        void highlight(const QString& text, const QString &stringregex, const QTextCharFormat &charformat);
        void highlightSegment(const QString& text);
        void highlightFunction(const QString& text);
        void highlightComment(const QString& text);
        void highlightLabel(const QString& text);

    protected:
        virtual void highlightBlock(const QString &text);

    private:
        Block* _block;
        QTextCharFormat _segmentformat;
        QTextCharFormat _segmentkwformat;
        QTextCharFormat _functionformat;
        QTextCharFormat _functionnameformat;
        QTextCharFormat _hexdigitsformat;
        QTextCharFormat _commentformat;
        QTextCharFormat _jumplabelformat;
};

#endif // DISASSEMBLERHIGHLIGHTER_H
