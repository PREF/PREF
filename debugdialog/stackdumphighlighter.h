#ifndef STACKDUMPHIGHLIGHTER_H
#define STACKDUMPHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class StackDumpHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    public:
        explicit StackDumpHighlighter(QTextDocument *parent = 0);

    private:
        void highlight(const QString& text, const QString& stringregex, const QTextCharFormat& charformat);

    protected:
        virtual void highlightBlock(const QString &text);

    private:
        QTextCharFormat _digitsformat;
        QTextCharFormat _keywordformat;
        QTextCharFormat _stringformat;
        QTextCharFormat _stackidxformat;
};

#endif // STACKDUMPHIGHLIGHTER_H
