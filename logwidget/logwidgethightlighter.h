#ifndef LOGWIDGETHIGHTLIGHTER_H
#define LOGWIDGETHIGHTLIGHTER_H

#include <QtCore>
#include <QtGui>

class LogWidgetHightlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    public:
        explicit LogWidgetHightlighter(QTextDocument *parent);

    protected:
        virtual void highlightBlock(const QString &text);

    private:
        QTextCharFormat _defaultformat;
        QTextCharFormat _noticecharformat;
        QTextCharFormat _warningcharformat;
        QTextCharFormat _errorcharformat;
};

#endif // LOGWIDGETHIGHTLIGHTER_H
