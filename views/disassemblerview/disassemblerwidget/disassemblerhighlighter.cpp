#include "disassemblerhighlighter.h"

DisassemblerHighlighter::DisassemblerHighlighter(QTextDocument *parent): QSyntaxHighlighter(parent)
{
    this->_segmentformat.setForeground(Qt::darkGreen);

    this->_segmentkwformat.setForeground(Qt::darkGreen);
    this->_segmentkwformat.setFontWeight(QFont::Bold);

    this->_functionformat.setForeground(Qt::blue);
    this->_functionformat.setFontWeight(QFont::Bold);

    this->_functionnameformat.setForeground(Qt::darkRed);

    this->_currentaddresscharformat.setForeground(Qt::darkBlue);
    this->_hexdigitsformat.setForeground(Qt::darkBlue);

    this->_registersformat.setForeground(Qt::darkBlue);
    this->_registersformat.setFontWeight(QFont::Bold);

    this->_commentformat.setFontItalic(true);
    this->_commentformat.setForeground(Qt::darkGreen);
}

void DisassemblerHighlighter::highlight(const QString &text, const QString& stringregex, const QTextCharFormat& charformat)
{
    QRegExp regex(stringregex);
    int idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, len, charformat);
        idx = text.indexOf(regex, idx + len);
    }
}

void DisassemblerHighlighter::highlightSegment(const QString &text)
{
    QRegExp regex("segment [^.]+$");
    int idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, 7, this->_segmentkwformat);
        this->setFormat(idx + 7, len - 7, this->_segmentformat);
        idx = text.indexOf(regex, idx + len);
    }
}

void DisassemblerHighlighter::highlightFunction(const QString &text)
{
    QRegExp regex("[a-z]* function [^.]+\\(\\)[^.]+$");
    int idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, len, this->_functionformat);
        idx = text.indexOf(regex, idx + len);
    }

    regex = QRegExp("[^. ]+\\(\\)");
    idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, len, this->_functionnameformat);
        idx = text.indexOf(regex, idx + len);
    }
}

void DisassemblerHighlighter::highlightComment(const QString &text)
{
    QRegExp regex("#[^.]+$");
    int idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, len, this->_commentformat);
        idx = text.indexOf(regex, idx + len);
    }
}

void DisassemblerHighlighter::highlightBlock(const QString &text)
{
    if(text.isEmpty())
        return;

    this->highlightSegment(text);
    this->highlightFunction(text);

    this->highlight(text, "[0-9a-fA-F]+h", this->_hexdigitsformat);
    this->highlight(text, "\\$[a-zA-Z0-9]+", this->_registersformat);
    this->highlight(text, "[^.]*:[0-9a-fA-F]+", this->_currentaddresscharformat);

    this->highlightComment(text);
}
