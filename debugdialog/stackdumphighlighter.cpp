#include "stackdumphighlighter.h"

StackDumpHighlighter::StackDumpHighlighter(QTextDocument *parent): QSyntaxHighlighter(parent)
{
    this->_digitsformat.setFontFamily("Monospace");
    this->_digitsformat.setFontStyleHint(QFont::TypeWriter);
    this->_digitsformat.setForeground(Qt::darkBlue);

    this->_keywordformat.setFontFamily("Monospace");
    this->_keywordformat.setFontStyleHint(QFont::TypeWriter);
    this->_keywordformat.setForeground(Qt::blue);
    this->_keywordformat.setFontWeight(QFont::Bold);

    this->_stringformat.setFontFamily("Monospace");
    this->_stringformat.setFontStyleHint(QFont::TypeWriter);
    this->_stringformat.setForeground(Qt::darkGreen);

    this->_stackidxformat.setFontFamily("Monospace");
    this->_stackidxformat.setFontStyleHint(QFont::TypeWriter);
    this->_stackidxformat.setForeground(Qt::red);
    this->_stackidxformat.setFontWeight(QFont::Bold);
}

void StackDumpHighlighter::highlight(const QString &text, const QString &stringregex, const QTextCharFormat &charformat)
{
    int idx = 0;
    QRegExp regex(stringregex);

    while((idx = text.indexOf(regex, idx + qMax(0, regex.matchedLength()))) != -1)
        this->setFormat(idx, regex.matchedLength(), charformat);
}

void StackDumpHighlighter::highlightBlock(const QString &text)
{
    this->highlight(text, "[0-9]+", this->_digitsformat);
    this->highlight(text, "\\[[0-9]+\\]", this->_stackidxformat);
    this->highlight(text, "\\bstring|number|userdata|table|function|boolean|nil\\b", this->_keywordformat);
    this->highlight(text, "'.+'", this->_stringformat);
}
