#include "loghighlighter.h"
#include <QRegularExpression>

LogHighlighter::LogHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    this->_digitscharformat.setForeground(QColor(Qt::darkBlue));
    this->_stringcharformat.setForeground(QColor(Qt::darkGreen));
}

void LogHighlighter::highlightBlock(const QString &text)
{
    this->highlight(text, "(^|[ \\t]+)[\\+|\\-]*[0-9]+[\\.]*[0-9]*", this->_digitscharformat);
    this->highlight(text, "[\"|\'][^\'|\"]*[\"|\']", this->_stringcharformat);
}

void LogHighlighter::highlight(const QString &text, const QString &pattern, const QTextCharFormat& format)
{
    QRegularExpression regex(pattern);
    QRegularExpressionMatchIterator it = regex.globalMatch(text);

    while(it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), format);
    }
}
