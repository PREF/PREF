#include "disassemblerhighlighter.h"

DisassemblerHighlighter::DisassemblerHighlighter(QTextDocument *parent, Block* block): QSyntaxHighlighter(parent), _block(block)
{
    this->generateHighlighters();
}

void DisassemblerHighlighter::generateHighlighters()
{
    this->_segmentformat.setFontFamily("Monospace");
    this->_segmentformat.setFontStyleHint(QFont::TypeWriter);
    this->_segmentformat.setForeground(Qt::darkGreen);

    this->_segmentkwformat.setFontFamily("Monospace");
    this->_segmentkwformat.setFontStyleHint(QFont::TypeWriter);
    this->_segmentkwformat.setFontWeight(QFont::Bold);
    this->_segmentkwformat.setForeground(Qt::darkGreen);

    this->_functionformat.setFontFamily("Monospace");
    this->_functionformat.setFontStyleHint(QFont::TypeWriter);
    this->_functionformat.setFontWeight(QFont::Bold);
    this->_functionformat.setForeground(Qt::blue);

    this->_functionnameformat.setFontFamily("Monospace");
    this->_functionnameformat.setFontStyleHint(QFont::TypeWriter);
    this->_functionnameformat.setForeground(Qt::darkRed);

    this->_hexdigitsformat.setFontFamily("Monospace");
    this->_hexdigitsformat.setFontStyleHint(QFont::TypeWriter);
    this->_hexdigitsformat.setForeground(Qt::darkBlue);

    this->_commentformat.setFontFamily("Monospace");
    this->_commentformat.setFontStyleHint(QFont::TypeWriter);
    this->_commentformat.setFontItalic(true);
    this->_commentformat.setForeground(Qt::darkGreen);

    this->_jumplabelformat.setFontFamily("Monospace");
    this->_jumplabelformat.setFontStyleHint(QFont::TypeWriter);
    this->_jumplabelformat.setForeground(Qt::darkGray);
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
    QRegExp regex("[a-z]*[ ]*function [^.]+\\([^.]*\\)[^.]*$");
    int idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, len, this->_functionformat);
        idx = text.indexOf(regex, idx + len);
    }

    regex = QRegExp("[^. ]+\\([^.]*\\)");
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
    QRegExp regex("#[^#]*$");
    int idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, len, this->_commentformat);
        idx = text.indexOf(regex, idx + len);
    }
}

void DisassemblerHighlighter::highlightLabel(const QString &text)
{
    QRegExp regex("[a-zA-Z_]+[a-zA-Z0-9]*_[a-zA-Z0-9]+[\\:]*");
    int idx = text.indexOf(regex);

    if(idx == -1)
        return;

    this->setFormat(idx, regex.matchedLength(), this->_jumplabelformat);
}

void DisassemblerHighlighter::highlightBlock(const QString &text)
{
    if(this->_block->blockType() == Block::SegmentBlock)
        this->highlightSegment(text);
    else if(this->_block->blockType() == Block::FunctionBlock)
        this->highlightFunction(text);
    else if(this->_block->blockType() == Block::LabelBlock)
        this->highlightLabel(text);

    this->highlightComment(text);
    this->highlight(text, "\\b[0-9a-fA-F]+h\\b", this->_hexdigitsformat);
}
