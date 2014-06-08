#ifndef DISASSEMBLERHIGHLIGHTER_H
#define DISASSEMBLERHIGHLIGHTER_H

#include <QtCore>
#include <QtGui>

class DisassemblerHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    private:
        enum Construct
        {
            HexDigits,
        };

    public:
        explicit DisassemblerHighlighter(QTextDocument *parent);

    private:
        void highlight(const QString& text, const QString &stringregex, const QTextCharFormat &charformat);
        void highlightSegment(const QString& text);
        void highlightFunction(const QString& text);
        void highlightComment(const QString& text);

    protected:
        virtual void highlightBlock(const QString &text);

    private:
        QTextCharFormat _segmentformat;
        QTextCharFormat _segmentkwformat;
        QTextCharFormat _functionformat;
        QTextCharFormat _functionnameformat;
        QTextCharFormat _currentaddresscharformat;
        QTextCharFormat _hexdigitsformat;
        QTextCharFormat _registersformat;
        QTextCharFormat _commentformat;
};

#endif // DISASSEMBLERHIGHLIGHTER_H
