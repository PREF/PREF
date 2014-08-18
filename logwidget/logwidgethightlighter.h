#ifndef LOGWIDGETHIGHTLIGHTER_H
#define LOGWIDGETHIGHTLIGHTER_H

#include <QtCore>
#include <QtGui>

class LogWidgetHightlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    public:
        explicit LogWidgetHightlighter(QTextDocument *parent);

    private:
        void highlightSpecialLine(const QString& text, const QString& keyword, const QTextCharFormat& charformat);

    protected:
        virtual void highlightBlock(const QString &text);
        virtual void highlightStrings(const QString& text);
        virtual void highlightDigits(const QString& text);

    private:
        QTextCharFormat _noticecharformat;
        QTextCharFormat _warningcharformat;
        QTextCharFormat _errorcharformat;
        QTextCharFormat _stringcharformat;
        QTextCharFormat _digitscharformat;
};

#endif // LOGWIDGETHIGHTLIGHTER_H
