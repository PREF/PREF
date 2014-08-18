#include "logwidgethightlighter.h"

LogWidgetHightlighter::LogWidgetHightlighter(QTextDocument *parent): QSyntaxHighlighter(parent)
{
    this->_noticecharformat.setFontFamily("Monospace");
    this->_noticecharformat.setFontStyleHint(QFont::TypeWriter);
    this->_noticecharformat.setFontWeight(QFont::Bold);
    this->_noticecharformat.setForeground(QColor::fromRgb(0x22, 0x8B, 0x22));

    this->_warningcharformat.setFontFamily("Monospace");
    this->_warningcharformat.setFontStyleHint(QFont::TypeWriter);
    this->_warningcharformat.setFontWeight(QFont::Bold);
    this->_warningcharformat.setForeground(QColor::fromRgb(0xFF, 0xA5, 0x00));

    this->_errorcharformat.setFontFamily("Monospace");
    this->_errorcharformat.setFontStyleHint(QFont::TypeWriter);
    this->_errorcharformat.setFontWeight(QFont::Bold);
    this->_errorcharformat.setForeground(Qt::red);

    this->_stringcharformat.setFontFamily("Monospace");
    this->_stringcharformat.setFontStyleHint(QFont::TypeWriter);
    this->_stringcharformat.setForeground(QColor::fromRgb(0x80, 0x00, 0x00));
}

void LogWidgetHightlighter::highlightSpecialLine(const QString &text, const QString &keyword, const QTextCharFormat &charformat)
{
    QRegExp regex(QString("^%1:").arg(keyword));
    int idx = text.indexOf(regex);

    if(idx == -1)
        return;

    this->setFormat(idx, regex.matchedLength(), charformat);
}

void LogWidgetHightlighter::highlightBlock(const QString &text)
{
    if(text.isEmpty())
        return;

    this->highlightSpecialLine(text, "NOTICE", this->_noticecharformat);
    this->highlightSpecialLine(text, "WARNING", this->_warningcharformat);
    this->highlightSpecialLine(text, "ERROR", this->_errorcharformat);
    this->highlightDigits(text);
    this->highlightStrings(text);
}

void LogWidgetHightlighter::highlightStrings(const QString &text)
{
    int idx = 0;
    QRegExp regex("[\"|\'][^\'|\"]*[\"|\']");

    while((idx = text.indexOf(regex, idx + qMax(0, regex.matchedLength()))) != -1)
        this->setFormat(idx, regex.matchedLength(), this->_stringcharformat);
}

void LogWidgetHightlighter::highlightDigits(const QString &text)
{
    int idx = 0;
    QRegExp regex("[0-9A-F]+[hH]");

    while((idx = text.indexOf(regex, idx + qMax(0, regex.matchedLength()))) != -1)
        this->setFormat(idx, regex.matchedLength(), this->_digitscharformat);

    idx = 0;
    regex = QRegExp("(^|[ \\t]+)[\\+|\\-]*[0-9]+[\\.]*[0-9]*");

    while((idx = text.indexOf(regex, idx + qMax(0, regex.matchedLength()))) != -1)
        this->setFormat(idx, regex.matchedLength(), this->_digitscharformat);
}
