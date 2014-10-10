#ifndef DISASSEMBLERHIGHLIGHTER_H
#define DISASSEMBLERHIGHLIGHTER_H

#include <QtCore>
#include <QtGui>
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "prefsdk/disassembler/blocks/block.h"
#include "prefsdk/disassembler/blocks/instruction.h"

using namespace PrefSDK;

class DisassemblerHighlighter : public QSyntaxHighlighter
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
        void highlightInstruction(const QString& text);
        void highlightJumpLabel(const QString& text);
        void highlightSymbol(const QString& text);

    protected:
        virtual void highlightBlock(const QString &text);

    private:
        Block* _block;
        QTextCharFormat _segmentformat;
        QTextCharFormat _segmentkwformat;
        QTextCharFormat _functionformat;
        QTextCharFormat _functionnameformat;
        QTextCharFormat _hexdigitsformat;
        QTextCharFormat _registersformat;
        QTextCharFormat _commentformat;
        QTextCharFormat _jumplabelformat;
        QTextCharFormat _symbollabelformat;
};

#endif // DISASSEMBLERHIGHLIGHTER_H
