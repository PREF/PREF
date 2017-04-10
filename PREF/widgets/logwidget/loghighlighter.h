#ifndef LOGHIGHLIGHTER_H
#define LOGHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class LogHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    public:
        explicit LogHighlighter(QTextDocument *parent = 0);

    protected:
        virtual void highlightBlock(const QString &text);
        void highlight(const QString& text, const QString& pattern, const QTextCharFormat &format);

    private:
        QTextCharFormat _stringcharformat;
        QTextCharFormat _digitscharformat;
};

#endif // LOGHIGHLIGHTER_H
